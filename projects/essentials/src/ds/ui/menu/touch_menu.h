#pragma once
#ifndef DS_UI_MENU_TOUCH_MENU
#define DS_UI_MENU_TOUCH_MENU

#include <ds/ui/sprite/sprite.h>
#include <ds/touch/five_finger_cluster.h>

namespace ds {
namespace ui {
class ClusterView;

/*
* TouchMenu is the short name for a Five Finger Touch Menu.
* This is a menu that appears when 4 or more fingers are placed on the screen at any spot.
* When the menu appears, the user slides their whole hand to the selected option and releases.
* This class manages the touch clusters (parsing touch input into nearby points in FiveFingerCluster),
* then loading a view when enough fingers are onscreen and close enough together (a ClusterView)
* The ClusterView is where the actual five finger menu ui is created.
*
* TODO: add more "blingy" graphic support
* TODO: add useage comments
*
* Note: Intended to be used in Ortho roots only
*/
class TouchMenu : public ds::ui::Sprite {
public:

	/** A simple construction for setting up each item in the menu. */
	struct MenuItemModel {
		MenuItemModel() : mTitle(L""), mIconNormalImage(""), mIconHighlightedImage(""), mActivatedCallback(nullptr){}
		MenuItemModel(const std::wstring& titley, const std::string& normalImage = "", const std::string& highImage = "", std::function<void(ci::Vec3f)> callback = nullptr) 
			: mTitle(titley), mIconNormalImage(normalImage), mIconHighlightedImage(highImage), mActivatedCallback(callback){}

		std::function<void(ci::Vec3f)>		mActivatedCallback;			// Called when this item has been activated, the Vec3f is the position of the item
		std::wstring						mTitle;						// Label text for the menu item
		std::string							mIconNormalImage;			// Path to the normal image for the icon
		std::string							mIconHighlightedImage;		// Path to the highlighted image for the icon, uses normal image if blank
	};

	/** A structure for configuring a touch menu */
	struct TouchMenuConfig {

		TouchMenuConfig() 
			: mAnimationDuration(0.35f)
			, mItemIconHeight(150.0f), mItemTitlePad(20.0f), mItemTitleYPositionPercent(0.5f), mItemSize(250.0f, 250.0f), mItemTitleTextConfig("")
			, mClusterRadius(280.0f), mClusterPositionOffset(-90.0f)
			, mClusterSizeThreshold(1000.0f), mClusterDistanceThreshold(1000.0f)
			, mBackgroundImage(""), mBackgroundColor(0.0f, 0.0f, 0.0f), mBackgroundOpacity(0.3f), mBackgroundScale(3.0f)
		{}

		float								mAnimationDuration;			// duration of all animations

		float								mItemIconHeight;			// The height of the icon for each menu item, in pixels
		float								mItemTitlePad;				// Distance between the icon and the title in each menu item
		float								mItemTitleYPositionPercent; // Vertical Position of the title, as a percentage of the height of the menu item
		ci::Vec2f							mItemSize;					// size of each menu item
		std::string							mItemTitleTextConfig;		// The text config for the title of the menu item

		float								mClusterRadius;				// How large the overall cluster menu is
		float								mClusterPositionOffset;		// Rotation around the center of the menu the first item is placed at, in degrees

		float								mClusterSizeThreshold;		// How large the cluster of fingers can be before it is invalidated
		float								mClusterDistanceThreshold;	// How far from the start point the cluster can be dragged before it is invalidated

		std::string							mBackgroundImage;			// The path to an image 
		ci::Color							mBackgroundColor;			// Color to set the background image
		float								mBackgroundOpacity;			// Max opacity for the background image when the cluster is active
		float								mBackgroundScale;			// Scale of the background image when cluster is active

	};

	TouchMenu(ds::ui::SpriteEngine& enginey);

	/** Set a vector of models, one for each item in the menu. Clears any currently active cluster views. */
	void									setMenuItemModels(std::vector<MenuItemModel> itemModels);

	/** Sets the configuration for the touch menu. Clears any currently active cluster views. */
	void									setMenuConfig(TouchMenuConfig touchMenuConfig);

	/** Send all touch info (straight from the engine in most cases) directly here to be parsed */
	void									handleTouchInfo(ds::ui::Sprite* bs, const ds::ui::TouchInfo& ti);

private:
	void									handleClusterUpdate(const ds::ui::TouchInfo::Phase tp, const ds::ui::FiveFingerCluster::Cluster& clustersLastStand);
	void									clearClusters();

	ds::ui::FiveFingerCluster				mFiveFingerCluster;
	std::vector<ds::ui::ClusterView*>		mClusterViews;
	std::map<int, ds::ui::ClusterView*>		mClusterLookup;
	
	std::vector<MenuItemModel>				mMenuModels;
	TouchMenuConfig							mTouchMenuConfig;

};
} // namespace ui
} // namespace ds

#endif