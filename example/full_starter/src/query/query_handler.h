#pragma once
#ifndef QUERY_QUERYHANDLER_H_
#define QUERY_QUERYHANDLER_H_

#include <ds/app/event_client.h>
#include <ds/data/resource_list.h>
#include <ds/thread/serial_runnable.h>
#include <ds/ui/sprite/sprite_engine.h>
// NOTE: Placing this include at the top gets a conflict
// with cinder. Need to look into that.
#include <ds/network/node_watcher.h>
#include "query/story_query.h"
#include "model/all_stories.h"
#include "model/all_data.h"

namespace fullstarter {

/**
 * \class fullstarter::QueryHandler
 * \brief Handle app events that deal with querying for data.
 */
class QueryHandler {
public:
	QueryHandler(ds::ui::SpriteEngine&, AllData&);

private:

	void								onAppEvent(const ds::Event&);
	void								onStoryQuery(StoryQuery&);

	ds::EventClient						mEventClient;

	AllStories&							mAllStories;

	ds::SerialRunnable<StoryQuery>		mStoryQuery;

	// CACHING
	ds::ResourceList					mResources;
};

} // namespace fullstarter

#endif