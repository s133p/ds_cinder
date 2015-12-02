#include "layout_sprite.h"

#include <ds/app/environment.h>
#include <ds/ui/sprite/sprite_engine.h>
#include <ds/debug/logger.h>
#include <ds/ui/sprite/multiline_text.h>
#include <ds/ui/sprite/image.h>
#include <ds/util/string_util.h>


namespace ds {
namespace ui {

LayoutSprite::LayoutSprite(ds::ui::SpriteEngine& engine)
	: ds::ui::Sprite(engine)
	, mLayoutType(kLayoutVFlow)
	, mSpacing(0.0f)
{

}

void LayoutSprite::runLayout(){
	if(mLayoutType == kLayoutNone){
		runNoneLayout();
	} else if(mLayoutType == kLayoutVFlow){
		runFlowLayout(true);
	} else if(mLayoutType == kLayoutHFlow){
		runFlowLayout(false);
	} else if(mLayoutType == kLayoutSize){
		runSizeLayout();
	}

	onLayoutUpdate();
}

void LayoutSprite::runNoneLayout(){
	std::vector<ds::ui::Sprite*>& chillins = getChildren();
	for(auto it = chillins.begin(); it < chillins.end(); ++it){
		ds::ui::Sprite* chillin = (*it);
		auto layoutSprite = dynamic_cast<LayoutSprite*>(chillin);
		if(layoutSprite){
			layoutSprite->runLayout();
		}
	}
}

void LayoutSprite::runSizeLayout(){

	std::vector<ds::ui::Sprite*>& chillins = getChildren();
	const float layoutWidth = getWidth();
	const float layoutHeight = getHeight();

	// Size layout just adjusts the size of child elements to fit, if specified.
	// If children should ignore the size, don't add a layout size and set them to fixed.
	// Otherwise, stretch and flex both "gracefully" match sprites to the size of this layout
	for(auto it = chillins.begin(); it < chillins.end(); ++it){
		ds::ui::Sprite* chillin = (*it);

		if(chillin->mLayoutUserType == kFixedSize){
			if(chillin->mLayoutSize.x > 0.0f && chillin->mLayoutSize.y > 0.0f){
				ds::ui::MultilineText* mt = dynamic_cast<ds::ui::MultilineText*>(chillin);
				ds::ui::Image* img = dynamic_cast<ds::ui::Image*>(chillin);
				if(mt){
					mt->setResizeLimit(chillin->mLayoutSize.x, chillin->mLayoutSize.y);
				} else if(img){
					// restore position after calculating the box size
					ci::Vec3f prePos = img->getPosition();
					fitInside(img, ci::Rectf(0.0f, 0.0f, chillin->mLayoutSize.x, chillin->mLayoutSize.y), true);
					img->setPosition(prePos);
				} else {
					chillin->setSize(mLayoutSize);
				}
			}
		} else if(chillin->mLayoutUserType == kStretchSize || chillin->mLayoutUserType == kFlexSize || chillin->mLayoutUserType == kFillSize){
			const float fixedW = layoutWidth - chillin->mLayoutLPad - chillin->mLayoutRPad;
			const float fixedH = layoutHeight - chillin->mLayoutTPad - chillin->mLayoutBPad;

			ds::ui::MultilineText* mt = dynamic_cast<ds::ui::MultilineText*>(chillin);
			ds::ui::Image* img = dynamic_cast<ds::ui::Image*>(chillin);
			LayoutSprite* ls = dynamic_cast<LayoutSprite*>(chillin);
			if(mt){
				mt->setResizeLimit(fixedW, fixedH);
			} else if(img){
				// restore position after calculating the box size
				ci::Vec3f prePos = img->getPosition();
				fitInside(img, ci::Rectf(0.0f, 0.0f, fixedW, fixedH), true);
				img->setPosition(prePos);
			} else if(ls){
				ls->setSize(fixedW, fixedH);
				ls->runLayout();
			} else {
				chillin->setSize(fixedW, fixedH);
			}
		}


		LayoutSprite* ls = dynamic_cast<LayoutSprite*>(chillin);
		if(ls){
			ls->runLayout();
		}
	}

}

void LayoutSprite::runFlowLayout(const bool vertical){
	bool hasFills = false;
	int numStretches = 0;
	float totalSize = 0.0f;
	const float layoutWidth = getWidth();
	const float layoutHeight = getHeight();
	float maxSize = (vertical ? layoutHeight : layoutWidth);
	
	std::vector<ds::ui::Sprite*>& chillins = getChildren();

	// Look through all children to determine total size and how many items are set to stretch to fill the space
	// Also run recursive layouts on any non-stretch layouts and size any flexible items
	int spacedChildren = 0;

	for(auto it = chillins.begin(); it < chillins.end(); ++it){
		ds::ui::Sprite* chillin = (*it);

		if(chillin->mLayoutUserType == kFillSize){
			hasFills = true;
		} else {
			spacedChildren++;
			if(chillin->mLayoutUserType == kStretchSize){
				numStretches++;
			} else {
				if(chillin->mLayoutUserType == kFixedSize){
					if(chillin->mLayoutSize.x > 0.0f && chillin->mLayoutSize.y > 0.0f){
						ds::ui::MultilineText* mt = dynamic_cast<ds::ui::MultilineText*>(chillin);
						ds::ui::Image* img = dynamic_cast<ds::ui::Image*>(chillin);
						if(mt){
							mt->setResizeLimit(chillin->mLayoutSize.x, chillin->mLayoutSize.y);
						} else if(img){
							fitInside(img, ci::Rectf(0.0f, 0.0f, chillin->mLayoutSize.x, chillin->mLayoutSize.y), true);
						} else {
							chillin->setSize(mLayoutSize);
						}
					}

					LayoutSprite* ls = dynamic_cast<LayoutSprite*>(chillin);
					if(ls){
						ls->runLayout();
					}
				} else if(chillin->mLayoutUserType == kFlexSize){
					float fixedW = layoutWidth - chillin->mLayoutLPad - chillin->mLayoutRPad;
					float fixedH = layoutHeight - chillin->mLayoutTPad - chillin->mLayoutBPad;

					ds::ui::MultilineText* mt = dynamic_cast<ds::ui::MultilineText*>(chillin);
					ds::ui::Image* img = dynamic_cast<ds::ui::Image*>(chillin);
					LayoutSprite* ls = dynamic_cast<LayoutSprite*>(chillin);
				
					if(mt){
						if(vertical){
							mt->setResizeLimit(fixedW);
						} else {
							mt->setResizeLimit(mt->getResizeLimitWidth(), fixedH);
						}
					} else if(img){
						if(vertical){
							img->setScale(fixedW / img->getWidth());
						} else {
							img->setScale(fixedH / img->getHeight());
						}
					} else if(ls){
						if(vertical){
							ls->setSize(fixedW, ls->getHeight());
						} else {
							ls->setSize(ls->getWidth(), fixedH);
						}
						ls->runLayout();
					} else {
						if(vertical){
							chillin->setSize(fixedW, chillin->getHeight());
						} else {
							chillin->setSize(chillin->getWidth(), fixedH);
						}
					}
				}

				if(vertical){
					totalSize += chillin->mLayoutTPad + chillin->getScaleHeight() + chillin->mLayoutBPad;
				} else {
					totalSize += chillin->mLayoutLPad + chillin->getScaleWidth() + chillin->mLayoutRPad;
				}
			}
		}
	}

	if(spacedChildren > 1){
		totalSize += (mSpacing * (spacedChildren - 1));
	}

	float leftOver = 0.0f;
	float perStretch = 0.0f;
	if(numStretches > 0){
		leftOver = maxSize - totalSize;
		perStretch = leftOver / numStretches;
	}

	// Now that we know the size and leftover size, go through the children again, set position for all children 
	// and set the size of any stretch children
	float offset = 0.0f;
	if(mOverallAlign == kMiddle || mOverallAlign == kCenter){
		offset = maxSize / 2.0f - totalSize / 2.0f;
	} else if(mOverallAlign == kBottom || mOverallAlign == kRight){
		offset = maxSize - totalSize;
	}

	for(auto it = chillins.begin(); it < chillins.end(); ++it){
		ds::ui::Sprite* chillin = (*it);

		if(chillin->mLayoutUserType == kFillSize) {
			// Fill size only uses padding and fudge, but doesn't contribute to the flow
			chillin->setPosition(chillin->mLayoutLPad + chillin->mLayoutFudge.x, chillin->mLayoutTPad + chillin->mLayoutFudge.y);
			continue;
		}
		
		if(chillin->mLayoutUserType == kStretchSize){
			const float stretchW = (vertical ? layoutWidth : perStretch) - chillin->mLayoutLPad - chillin->mLayoutRPad;
			const float stretchH = (vertical ? perStretch : layoutHeight) - chillin->mLayoutTPad - chillin->mLayoutBPad;

			ds::ui::MultilineText* mt = dynamic_cast<ds::ui::MultilineText*>(chillin);
			ds::ui::Image* img = dynamic_cast<ds::ui::Image*>(chillin);
			LayoutSprite* ls = dynamic_cast<LayoutSprite*>(chillin);
			if(mt){
				mt->setResizeLimit(stretchW, stretchH);
			} else if(img){
				fitInside(img, ci::Rectf(0.0f, 0.0f, stretchW, stretchH), true);
			} else if(ls){
				ls->setSize(stretchW, stretchH);
				ls->runLayout();
			} else {
				chillin->setSize(stretchW, stretchH);
			}
		} 

		float xPos = 0.0f;
		float yPos = 0.0f;
		if(vertical){
			yPos = offset + chillin->mLayoutTPad;
			if(chillin->mLayoutHAlign == kLeft){
				xPos = chillin->mLayoutLPad;
			} else if(chillin->mLayoutHAlign == kCenter){
				xPos = layoutWidth / 2.0f - chillin->getScaleWidth() / 2.0f;
			} else if(chillin->mLayoutHAlign == kRight){
				xPos = layoutWidth - chillin->getScaleWidth() - chillin->mLayoutRPad;
			}
		} else {
			xPos = offset + chillin->mLayoutLPad;
			if(chillin->mLayoutVAlign == kTop){
				yPos = chillin->mLayoutTPad;
			} else if(chillin->mLayoutVAlign == kMiddle){
				yPos = layoutHeight / 2.0f - chillin->getScaleHeight() / 2.0f;
			} else if(chillin->mLayoutVAlign == kBottom){
				yPos = layoutHeight - chillin->getScaleHeight() - chillin->mLayoutBPad;
			}
		}
		
		chillin->setPosition(xPos + chillin->mLayoutFudge.x, yPos + chillin->mLayoutFudge.y);	
		
		if(vertical){
			offset += chillin->mLayoutTPad + chillin->getScaleHeight() + chillin->mLayoutBPad;
		} else {
			offset += chillin->mLayoutLPad + chillin->getScaleWidth() + chillin->mLayoutRPad;
		}

		offset += mSpacing;
	}

	if(spacedChildren > 1){
		offset -= mSpacing;
	}

	if(mLayoutUserType == kFlexSize){
		if(vertical){
			setSize(layoutWidth, offset);
		} else {
			setSize(offset, layoutHeight);
		}
	}

	if(hasFills){
		for(auto it = chillins.begin(); it < chillins.end(); ++it){
			ds::ui::Sprite* chillin = (*it);
			if(chillin->mLayoutUserType == kFillSize){
				const float fixedW = getWidth() - chillin->mLayoutLPad - chillin->mLayoutRPad;
				const float fixedH = getHeight() - chillin->mLayoutTPad - chillin->mLayoutBPad;

				ds::ui::MultilineText* mt = dynamic_cast<ds::ui::MultilineText*>(chillin);
				ds::ui::Image* img = dynamic_cast<ds::ui::Image*>(chillin);
				LayoutSprite* ls = dynamic_cast<LayoutSprite*>(chillin);
				if(mt){
					mt->setResizeLimit(fixedW, fixedH);
				} else if(img){
					fitInside(img, ci::Rectf(0.0f, 0.0f, fixedW, fixedH), true);
				} else if(ls){
					ls->setSize(fixedW, fixedH);
					ls->runLayout();
				} else {
					chillin->setSize(fixedW, fixedH);
				}
			}
		}
	}
}

void LayoutSprite::onLayoutUpdate(){
	if(mLayoutUpdatedFunction){
		mLayoutUpdatedFunction();
	}
}

void LayoutSprite::setLayoutUpdatedFunction(const std::function<void()> layoutUpdatedFunction){
	mLayoutUpdatedFunction = layoutUpdatedFunction;
}

void LayoutSprite::fitInside(ds::ui::Sprite* sp, const ci::Rectf area, const bool letterbox){
	if(!sp) return;
	// a = w / h;
	// h = w /a;
	// w = ah;
	const float spriteAspect = sp->getWidth() / sp->getHeight();
	const float areaAspect = area.getWidth() / area.getHeight();
	float destScale = sp->getScale().x;

	if(letterbox){
		// When letterboxing, if the sprite is narrower then the dest area, fill the height
		if(spriteAspect < areaAspect){
			destScale = area.getHeight() / sp->getHeight();
		} else {
			destScale = area.getWidth() / sp->getWidth();
		}
	} else {
		// When NOT letterboxing, if the sprite is wider then the dest area, fill the height
		if(spriteAspect > areaAspect){
			destScale = area.getHeight() / sp->getHeight();
		} else {
			destScale = area.getWidth() / sp->getWidth();
		}
	}

	sp->setScale(destScale, destScale, 1.0f);
	sp->setPosition(area.getX1() + area.getWidth() / 2.0f - sp->getScaleWidth() / 2.0f, area.getY1() + area.getHeight() / 2.0f - sp->getScaleHeight() / 2.0f);
}

} // namespace ui
} // namespace ds