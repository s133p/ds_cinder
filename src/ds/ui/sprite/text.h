#pragma once
#ifndef DS_UI_TEXT_H
#define DS_UI_TEXT_H
#include <cinder/gl/Texture.h>
#include <cinder/gl/TextureFont.h>
#include <cinder/Text.h>
#include <cinder/Font.h>
#include "ds/ui/sprite/sprite.h"
#include "ds/ui/sprite/text_layout.h"
#include "cinder/gl/Fbo.h"

namespace ds {
namespace ui {

/**
 * \class ds::ui::Text
 * A sprite for displaying text.  All layouts are generated by a layout
 * function.  See text_layout for all possible functions, or write your own.
 * Note that some functions require additional properties not on the sprite
 * (for example, spacing between lines, for multi-line layouts), in which
 * case the function is typically generated by an additional object responsible
 * for storing the info.
 */
class Text: public Sprite
{
    public:
        Text(SpriteEngine&);
        ~Text();

        // The text allows a lot of control between auto resizing and forcing specific
        // sizes.  In general, resizeToText will do exactly what it says, and resize
        // the sprite after a layout to match the text.  However, you can set a limit,
        // which will prevent the auto resize from going beyond a certain value.
        Text&                     setResizeToText(const bool = true);
        Text&                     setResizeToText(const bool width, const bool height);
        bool                      autoResizeWidth() const;
        bool                      autoResizeHeight() const;
        // When the limit is <= 0 it is unusued.  When it's a valid value, then it is
        // ignored if resize is OFF, but if resize is ON, it prevents the view from
        // being resized beyond the limit.
        float                     getResizeLimitWidth() const;
        float                     getResizeLimitHeight() const;
        Text&                     setResizeLimit(const float width = 0, const float height = 0);

        Text&                     setFont(const std::string& filename, const float fontSize);
        void                      setSizeAll( float width, float height, float depth );
        virtual float             getWidth() const;
        virtual float             getHeight() const;
        void                      setAlignment( int alignment );
        Text&                     setBorder(const ci::Rectf&);
        virtual void              updateServer(const UpdateParams&);
        void                      drawLocalClient();
        Text&                     setText( const std::string &text );
        std::string               getText() const;
        bool                      hasText() const;

        // Set a function for translating a string into a layout object.
        Text&                     setLayoutFunction(const TextLayout::MAKE_FUNC&);

        // Font metrics, this is probably temporary
        float                     getFontAscent() const;
        float                     getFontDescent() const;
        float                     getFontHeight() const;  // does not include leading
        float                     getFontLeading() const;

        // Print my line info
        void                      debugPrint();

    protected:
        bool                      mNeedsLayout;
        bool                      mNeedRedrawing;

    private:
        typedef Sprite inherited;

        void                      makeLayout();
        void                      drawIntoFbo();
        // Only used when ResizeToText is on
        void                      calculateFrame(const int flags);

        ci::gl::TextureFontRef    mTextureFont;
        ci::gl::TextureFont::DrawOptions
                                  mDrawOptions;
        
        float                     mFontSize;
        std::string               mTextString;
        ci::Rectf                 mBorder;

        // After performing the layout, this determines if my width and/or height
        // will be set to the layout bounds.
        int                       mResizeToTextF;
        TextLayout                mLayout;
        TextLayout::MAKE_FUNC     mLayoutFunc;
        float                     mResizeLimitWidth,
                                  mResizeLimitHeight;

        // When true, display the whole sprite area.
        const bool                mDebugShowFrame;

        ci::gl::Fbo               mFbo;
        ci::gl::Texture           mTexture;

        // Initialization
    public:
        static void                     installAsServer(ds::BlobRegistry&);
        static void                     installAsClient(ds::BlobRegistry&);
};

} // namespace ui
} // namespace ds

#endif//DS_UI_TEXT_H
