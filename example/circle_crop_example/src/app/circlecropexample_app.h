#ifndef _CIRCLECROPEXAMPLE_APP_H_
#define _CIRCLECROPEXAMPLE_APP_H_

#include <cinder/app/AppBasic.h>
#include <ds/app/app.h>

#include "app/globals.h"
#include "query/query_handler.h"

namespace example {
class AllData;

class CircleCropExample : public ds::App {
public:
	CircleCropExample();

	virtual void		keyDown(ci::app::KeyEvent event);
	void				setupServer();
	void				update();
private:
	typedef ds::App		inherited;

	// Data
	AllData				mAllData;

	// Data acquisition
	Globals				mGlobals;
	QueryHandler		mQueryHandler;

	//Idle state of the app to detect state change
	bool				mIdling;


	void				moveCamera(const glm::vec3& deltaMove);
};

} // !namespace example

#endif // !_CIRCLECROPEXAMPLE_APP_H_