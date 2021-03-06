#include <ds/app/event_registry.h>

#include <assert.h>
#include <iostream>
#include <sstream>
#include <unordered_map>
#include <Poco/Mutex.h>
#include <ds/debug/debug_defines.h>


namespace {
	const std::string& getEmptySz() {
		static const std::string EMPTY_SZ("");
		return EMPTY_SZ;
	}
}

namespace ds {

namespace event {

/**
 * \class ds::event::Registry
 */
Registry& Registry::get() {
	static Registry	REGISTRY;
	return REGISTRY;
}

Registry::Registry() {
}

int Registry::add(const std::string &name) {
	// For backwards compatibility with the old registry. Please tell
	// me no one has created this many old-style events.
	static const int		START(10000);
	int						ans = mMsgs.size() + 1 + START;
	mMsgs[ans] = name;
	return ans;
}

void Registry::report() {
	std::cout << "rf::msg::Registry size=" << mMsgs.size() << std::endl;
	for (auto it = mMsgs.begin(), end = mMsgs.end(); it != end; ++it) {
		std::cout << "\t" << it->first << ": " << it->second << std::endl;
	}
}

const std::string& Registry::getName(const int what){
	auto f = mMsgs.find(what);
	if(f != mMsgs.end()){
		return f->second;
	}

	return getEmptySz();
}

/**
 * \class ds::event::Registry::Entry
 */
Registry::Entry::Entry(const std::string &name)
		: mWhat(Registry::get().add(name))
		, mName(name) {
	std::stringstream		buf;
	buf << "event/" << mWhat;
	mChannel = buf.str();
}

} // namespace event


/**
 * DEPRECATED. Use the RegisteredEvent<> subclass instead.
 */

namespace {


// Currently, we are requiring lock protection on the event map,
// although really, this stuff should always happen during initialization

static Poco::Mutex& get_register_lock() {
	static Poco::Mutex  REGISTER_LOCK;
	return REGISTER_LOCK;
}

static std::unordered_map<int, std::string>& get_events() {
	static std::unordered_map<int, std::string>   EVENTS;
	return EVENTS;
}

// Old-style, where clients were supplying the what
static void register_event_deprecated(const int what, const std::string& name) {
	Poco::Mutex::ScopedLock   l(get_register_lock());
	auto& e = get_events();
	if (!e.empty()) {
		auto f = e.find(what);
		if (f != e.end()) {
			DS_DBG_CODE(std::cout << "ERROR Event::registerEvent() ALREADY REGISTERED " << what << " (requested " << name << ", have " << f->second << ")" << std::endl);
			assert(false);
			return;
		}
	}
	e[what] = name;
}

static int register_event(const std::string& name) {
	Poco::Mutex::ScopedLock		l(get_register_lock());
	auto&						e = get_events();
	int							what = e.size() + 1;
	DS_DBG_CODE(if (!e.empty()) {
		auto f = e.find(what);
		if (f != e.end()) {
			std::cout << "ERROR Event::registerEvent() ALREADY REGISTERED " << what << " (requested " << name << ", have " << f->second << ")" << std::endl;
			assert(false);
		}
	});
	e[what] = name;
	return what;
}

}

/**
 * \class ds::EventRegistry
 */
const std::string& EventRegistry::getName(const int what) {
	Poco::Mutex::ScopedLock   l(get_register_lock());
	auto& e = get_events();
	if (e.empty()) return getEmptySz();
	auto f = e.find(what);
	if (f != e.end()) return f->second;
	return event::Registry::get().getName(what);
	//return getEmptySz();
}

EventRegistry::EventRegistry(const std::string& name)
	: mWhat(register_event(name))
{
}

EventRegistry::EventRegistry(const int what, const std::string& name)
	: mWhat(register_event(name))
{
//	register_event(what, name);
}

} // namespace ds
