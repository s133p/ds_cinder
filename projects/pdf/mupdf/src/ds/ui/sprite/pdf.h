#pragma once
#ifndef DS_UI_SPRITE_PDF_H_
#define DS_UI_SPRITE_PDF_H_

#include <cinder/Surface.h>
#include <ds/ui/sprite/sprite.h>
#include "ds/data/resource.h"
namespace ds {
namespace pdf {
class PdfRes;
class Service;
}

namespace ui {

/**
 * \class ds::ui::sprite::Pdf
 */
class Pdf : public ds::ui::Sprite {
public:
	static Pdf&					makePdf(SpriteEngine&, Sprite* parent = nullptr);
	// Utility to get a render of the first page of a PDF.
	static ci::Surface8u		renderPage(const std::string& path);
	// Constant size will cause the sprite to size itself to the first PDF
	// and scale all subsequent PDF pages to match.
	// Auto resize will cause the view to resize when the page size changes.
	enum						PageSizeMode { kConstantSize, kAutoResize };

	Pdf(ds::ui::SpriteEngine&);

	Pdf&						setPageSizeMode(const PageSizeMode&);
	Pdf&						setResourceFilename(const std::string& filename);
	Pdf&						setResourceId(const ds::Resource::Id&);
	// Callback when the page size changes (only triggered in kAutoResize mode).
	void						setPageSizeChangedFn(const std::function<void(void)>&);

	virtual void				updateClient(const UpdateParams&);
	virtual void				updateServer(const UpdateParams&);

	// PDF API
	/** Displays the page given, from 1 to getPageCount() */
	void						setPageNum(const int pageNum);
	/** Returns the page currently being displayed, 1-indexed (page 1 is the first page, there is no page 0) */
	int							getPageNum() const;
	/** Returns the number of pages in this document. PDF's are 1-indexed, so the number returned from this function can be set as the last page */
	int							getPageCount() const;

	/** Advance the current page by 1, does not wrap */
	void						goToNextPage();
	/** Decrement the current page by 1, does not wrap */
	void						goToPreviousPage();

#ifdef _DEBUG
	virtual void				writeState(std::ostream&, const size_t tab) const;
#endif

protected:
	virtual void				onScaleChanged();
	virtual void				drawLocalClient();

	virtual void				writeAttributesTo(ds::DataBuffer&);
	virtual void				readAttributeFrom(const char attributeId, ds::DataBuffer&);

private:
	typedef ds::ui::Sprite		inherited;

	// STATE
	std::string					mResourceFilename;
	PageSizeMode				mPageSizeMode;
	std::function<void(void)>	mPageSizeChangeFn;
	// CACHE
	ci::Vec2i					mPageSizeCache;

	// It'd be nice just have the PdfRes in a unique_ptr,
	// but it has rules around destruction
	class ResHolder {
	public:
		ResHolder(ds::ui::SpriteEngine&);
		~ResHolder();

		void					clear();
		void					setResourceFilename(const std::string& filename, const PageSizeMode&);
		void					update();
		void					drawLocalClient();
		void					setScale(const ci::Vec3f&);
		void					setPageSizeMode(const PageSizeMode&);
		float					getWidth() const;
		float					getHeight() const;
		float					getTextureWidth() const;
		float					getTextureHeight() const;
		void					setPageNum(const int pageNum);
		int						getPageNum() const;
		int						getPageCount() const;
		ci::Vec2i				getPageSize() const;
		void					goToNextPage();
		void					goToPreviousPage();

	private:
		ds::pdf::Service&		mService;
		ds::pdf::PdfRes*		mRes;
	};
	ResHolder					mHolder;

	// Initialization
public:
	static void					installAsServer(ds::BlobRegistry&);
	static void					installAsClient(ds::BlobRegistry&);
};

} // namespace ui
} // namespace ds

#endif // DS_UI_SPRITE_PDF_H_