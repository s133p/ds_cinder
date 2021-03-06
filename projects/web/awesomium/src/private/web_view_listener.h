#pragma once
#ifndef PRIVATE_WEBVIEWLISTENER_H_
#define PRIVATE_WEBVIEWLISTENER_H_

#include <functional>
#include "Awesomium/WebURL.h"
#include "Awesomium/WebViewListener.h"
#include <Awesomium/WebMenuItem.h>

namespace ds {
namespace ui {
class Web;
}
namespace web {

/**
 * \class ds::web::WebViewListener
 * \brief Handle View callbacks.
 */
class WebViewListener : public Awesomium::WebViewListener::View {
public:
	WebViewListener(ds::ui::Web* web);
	virtual ~WebViewListener();

	void setAddressChangedFn(const std::function<void(const std::string& new_address)>&);

	virtual void OnChangeTitle(Awesomium::WebView* caller,
                             const Awesomium::WebString& title) { }
	virtual void OnChangeAddressBar(Awesomium::WebView* caller,
                                  const Awesomium::WebURL& url);
	virtual void OnChangeTooltip(Awesomium::WebView* caller,
                               const Awesomium::WebString& tooltip) { }
	virtual void OnChangeTargetURL(Awesomium::WebView* caller,
                                 const Awesomium::WebURL& url) { }
	virtual void OnChangeCursor(Awesomium::WebView* caller,
                              Awesomium::Cursor cursor) { }
	virtual void OnChangeFocus(Awesomium::WebView* caller,
                             Awesomium::FocusedElementType focused_type) { }
	virtual void OnAddConsoleMessage(Awesomium::WebView* caller,
                                   const Awesomium::WebString& message,
                                   int line_number,
                                   const Awesomium::WebString& source) { }
	virtual void OnShowCreatedWebView(Awesomium::WebView* caller,
                                    Awesomium::WebView* new_view,
                                    const Awesomium::WebURL& opener_url,
                                    const Awesomium::WebURL& target_url,
                                    const Awesomium::Rect& initial_pos,
                                    bool is_popup);

private:
	ds::ui::Web *mWeb;
	std::function<void(const std::string& new_address)>
									mAddressChangedFn;
};

/**
 * \class ds::web::WebLoadListener
 * \brief Handle Load callbacks.
 */
class WebLoadListener : public Awesomium::WebViewListener::Load {
public:
	WebLoadListener(ds::ui::Web* web);
	virtual ~WebLoadListener();

	void					setOnDocumentReady(const std::function<void(const std::string& url)>&);

	virtual void			OnBeginLoadingFrame(	Awesomium::WebView* caller, int64 frame_id,
													bool is_main_frame, const Awesomium::WebURL& url,
													bool is_error_page);
	virtual void			OnFailLoadingFrame(		Awesomium::WebView* caller, int64 frame_id,
													bool is_main_frame, const Awesomium::WebURL& url,
													int error_code, const Awesomium::WebString& error_desc);
	virtual void			OnFinishLoadingFrame(	Awesomium::WebView* caller, int64 frame_id,
													bool is_main_frame, const Awesomium::WebURL& url);
	virtual void			OnDocumentReady(		Awesomium::WebView* caller, const Awesomium::WebURL& url);

private:
	ds::ui::Web* mWeb;
	std::function<void(const std::string& url)>
							mOnDocumentReadyFn;
};

/**
* \class ds::web::WebProcessListener
* \brief Handle Process callbacks.
*/
class WebProcessListener : public Awesomium::WebViewListener::Process {
public:
	WebProcessListener(ds::ui::Web* web);
	virtual ~WebProcessListener();;
	/// This event occurs when the process hangs.
	virtual void OnUnresponsive(Awesomium::WebView* caller);

	/// This event occurs when the process becomes responsive after
	/// a hang.
	virtual void OnResponsive(Awesomium::WebView* caller);

	/// This event occurs when the process crashes.
	virtual void OnCrashed(Awesomium::WebView* caller,
						   Awesomium::TerminationStatus status);

	/// This even occurs when a new WebView render process is launched.
	virtual void OnLaunch(Awesomium::WebView* caller);

private:
	ds::ui::Web *mWeb;
};


/**
* \class ds::web::WebDialogListener
* \brief Handle Dialog callbacks.
*/
class WebDialogListener : public Awesomium::WebViewListener::Dialog {
public:
	WebDialogListener(ds::ui::Web* web);
	virtual ~WebDialogListener();
	/// This event occurs when the page requests to display a file chooser
	/// dialog. This is usually the result of a user clicking on an HTML
	/// input element with `type='file`. It is your responsibility to display
	/// this menu in your application. This event is not modal.
	///
	/// @see WebView::DidChooseFiles
	///
	virtual void OnShowFileChooser(Awesomium::WebView* caller,
								   const Awesomium::WebFileChooserInfo& chooser_info);

	///
	/// This event occurs when the page needs authentication from the user (for
	/// example, Basic HTTP Auth, NTLM Auth, etc). It is your responsibility to
	/// display a dialog so that users can input their username and password.
	/// This event is not modal.
	///
	/// @see WebView::DidLogin
	/// @see WebView::DidCancelLogin
	///
	virtual void OnShowLoginDialog(Awesomium::WebView* caller,
								   const Awesomium::WebLoginDialogInfo& dialog_info);

	///
	/// This event occurs when an SSL certificate error is encountered. This is
	/// equivalent to when Chrome presents a dark-red screen with a warning about
	/// a 'security certificate'. You may be able to ignore this error and
	/// continue loading the page if is_overridable is true.
	///
	/// @see WebView::DidOverrideCertificateError
	///
	virtual void OnShowCertificateErrorDialog(Awesomium::WebView* caller,
											  bool is_overridable,
											  const Awesomium::WebURL& url,
											  Awesomium::CertError error);

	///
	/// This event occurs as a result of an asynchronous call to 
	/// WebView::RequestPageInfo. You can use this event to display additional
	/// information about a page's SSL certificate or security status.
	///
	/// @see WebView::RequestPageInfo
	///
	virtual void OnShowPageInfoDialog(Awesomium::WebView* caller,
									  const Awesomium::WebPageInfo& page_info);

private:
	ds::ui::Web *mWeb;
};

/*
*\class ds::web::WebDialogListener
* \brief Handle Menu callbacks.
*/
class WebMenuListener : public Awesomium::WebViewListener::Menu {
public:
	///
	/// This event occurs when the page requests to display a dropdown
	/// (popup) menu. This is usually the result of a user clicking on
	/// a "select" HTML input element. It is your responsibility to
	/// display this menu in your application. This event is not modal.
	///
	/// @see WebView::DidSelectPopupMenuItem
	/// @see WebView::DidCancelPopupMenu
	///
	virtual void OnShowPopupMenu(Awesomium::WebView* caller,
								 const Awesomium::WebPopupMenuInfo& menu_info);

	///
	/// This event occurs when the page requests to display a context menu.
	/// This is usually the result of a user right-clicking somewhere on the
	/// page. It is your responsibility to display this menu in your
	/// application and perform the selected actions. This event is not modal.
	///
	virtual void OnShowContextMenu(Awesomium::WebView* caller,
								   const Awesomium::WebContextMenuInfo& menu_info);
};

} // namespace web
} // namespace ds

#endif // PRIVATE_WEBVIEWLISTENER_H_