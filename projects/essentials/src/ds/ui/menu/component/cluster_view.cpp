#include "cluster_view.h"

#include <ds/debug/logger.h>
#include <ds/ui/sprite/sprite_engine.h>
#include <ds/app/event_notifier.h>
#include <ds/app/engine/engine_cfg.h>
#include <ds/app/environment.h>
#include <ds/ui/tween/tweenline.h>

#include "ds/ui/menu/component/menu_item.h"

namespace ds{
namespace ui{

ClusterView::ClusterView(ds::ui::SpriteEngine& enginey, ds::ui::TouchMenu::TouchMenuConfig menuConfig, std::vector<ds::ui::TouchMenu::MenuItemModel>	itemModels)
	: ds::ui::Sprite(enginey)
	, mActive(false)
	, mBackground(nullptr)
	, mMenuConfig(menuConfig)
	, mItemModels(itemModels)
{

	enable(false);
	deactivate();

	if(!mMenuConfig.mBackgroundImage.empty()){
		mBackground = new ds::ui::Image(mEngine, mMenuConfig.mBackgroundImage);
		if(mBackground){
			addChild(*mBackground);
			mBackground->setCenter(0.5f, 0.5f);
			mBackground->setPosition(0.0f, 0.0f);
			mBackground->setColor(mMenuConfig.mBackgroundColor);
			mBackground->setScale(0.0f, 0.0f, 0.0f);
			mBackground->setOpacity(0.0f);
		}
	}


	buildMenuItems();
}

bool ClusterView::getActive(){
	return mActive;
}

void ClusterView::buildMenuItems(){

	for(auto it = mMenuItems.begin(); it < mMenuItems.end(); ++it){
		(*it)->release();
	}

	mMenuItems.clear();

	if(mItemModels.empty()){
		return;
	}

	float mis = (float)mItemModels.size();
	int i = 0;
	ci::Vec2f newPos = ci::Vec2f(mMenuConfig.mClusterRadius / 2.0f, -mMenuConfig.mItemSize.y * (mis) / 2.0f);
	for(auto it = mItemModels.begin(); it < mItemModels.end(); ++it){
		MenuItem* mi = new MenuItem(mEngine, (*it), mMenuConfig);
		addChildPtr(mi);
		mMenuItems.push_back(mi);

		float			degs = mMenuConfig.mClusterPositionOffset + ((float)(i)* 360.0f / (mis));
		while(degs >= 360.0f) degs -= 360.0f;

		const float		radians = -ci::toRadians(degs);
		newPos = ci::Vec2f(mMenuConfig.mClusterRadius * cos(radians), mMenuConfig.mClusterRadius * sin(radians));
		newPos.x -= mi->getWidth() / 2.0f;
		newPos.y -= mi->getHeight() / 2.0f;

		mi->setAngle(degs);
		mi->setPosition(newPos.x, newPos.y);
		i++;
	}

}

void ClusterView::activateMenu(){
	int dir(3);

	for(auto it = mMenuItems.begin(); it < mMenuItems.end(); ++it){
		(*it)->animateOn(dir);
	}
}

void ClusterView::setHighlight(ci::Vec2f clusterCenter){

	bool somethingHighlighted(false);
	for(auto it = mMenuItems.begin(); it < mMenuItems.end(); ++it){
		if((*it)->contains(ci::Vec3f(clusterCenter.x, clusterCenter.y, 0.0f))){
			(*it)->highlight();

			somethingHighlighted = true;

		} else {
			(*it)->unhighlight();
		}
	}
}


void ClusterView::updateCluster(const ds::ui::TouchInfo::Phase btp, const ds::ui::FiveFingerCluster::Cluster& cluster){
	if(!mActive
	   && cluster.mTouches.size() > 4
	   && validateCluster(cluster, false)){

		// TODO: send an event or seomthing
		//mGlobals.notify(FiveTouchInputEvent(cluster.mCurrentBoundingBox.getCenter()));

		activate();
		setPosition(cluster.mCurrentBoundingBox.getCenter().x, cluster.mCurrentBoundingBox.getCenter().y);
	}

	if(mActive && !mInvalid){

		// Make sure we've grabbed all the touches and they aren't doing stuff to other sprites.
		for(auto it = cluster.mTouches.begin(); it < cluster.mTouches.end(); ++it){
			ds::ui::Sprite* target = mEngine.getSpriteForFinger((*it).mFingerId);
			// If there's no target, then don't worry about it, since this sprite doesn't need to own the touch
			if(target && target != this){
				target->passTouchToSprite(this, (*it));
			}
		}

		ci::Vec2f clusterCenter = cluster.mCurrentBoundingBox.getCenter();
		setHighlight(clusterCenter);

		if(!validateCluster(cluster, true)){
			invalidate();
		}
	}

	if(btp == ds::ui::TouchInfo::Removed && !mMenuItems.empty()){
		for(auto it = mMenuItems.begin(); it < mMenuItems.end(); ++it){
			if((*it)->getHighlited()){
				itemActivated((*it));
				break;
			}
		}
		deactivate();
	}
}

void ClusterView::itemActivated(MenuItem* mi){
	ci::Vec3f launchPos = this->localToGlobal(mi->getCenter());

	if(mi && mi->getModel().mActivatedCallback){
		mi->getModel().mActivatedCallback(launchPos);
	}
}

bool ClusterView::validateCluster(const ds::ui::FiveFingerCluster::Cluster& cluster, const bool calcDist){

	float sizeThreshold = mMenuConfig.mClusterSizeThreshold;
	float distThreshold = mMenuConfig.mClusterDistanceThreshold;

	if(cluster.mCurrentBoundingBox.getWidth() > sizeThreshold || cluster.mCurrentBoundingBox.getHeight() > sizeThreshold){
		return false;
	}

	if(calcDist){
		ci::Vec2f bbcent = cluster.mCurrentBoundingBox.getCenter();
		ci::Vec3f center = getPosition();
		float xdelt = bbcent.x - center.x;
		float ydelt = bbcent.y - center.y;
		if(distThreshold * distThreshold < (xdelt * xdelt) + (ydelt * ydelt)){
			return false;
		}
	}

	return true;
}

void ClusterView::activate(){
	show();
	activateMenu();

	mActive = true;

	if(mBackground){
		float bgOpacity = mMenuConfig.mBackgroundOpacity;
		float bgScale = mMenuConfig.mBackgroundScale;
		mBackground->animStop();
		mBackground->tweenScale(ci::Vec3f(bgScale, bgScale, 1.0f), mMenuConfig.mAnimationDuration, 0.0f, ci::easeOutCubic);
		mBackground->tweenOpacity(bgOpacity, mMenuConfig.mAnimationDuration);
	}
}

void ClusterView::deactivate(){
	invalidate();
	mActive = false;
	mInvalid = false;
}

void ClusterView::invalidate(){
	if(!mInvalid){

		for(auto it = mMenuItems.begin(); it < mMenuItems.end(); ++it){
			(*it)->unhighlight();
			(*it)->animateOff(1);
		}

		if(mBackground){
			mBackground->animStop();
			mBackground->tweenScale(ci::Vec3f::zero(), mMenuConfig.mAnimationDuration, 0.0f, ci::easeInCubic, [this]{handleInvalidateComplete(); });
			mBackground->tweenOpacity(0.0f, mMenuConfig.mAnimationDuration);
		}
	}

	mInvalid = true;
}

void ClusterView::handleInvalidateComplete(){
	hide();
}

} // namespace ui
} // namespace ds