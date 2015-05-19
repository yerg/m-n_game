#include "Input.h"

Input::Input(){
	SDL_EventState(SDL_KEYDOWN,SDL_DISABLE);
	SDL_EventState(SDL_KEYUP,SDL_DISABLE);
	SDL_EventState(SDL_MOUSEMOTION,SDL_DISABLE);
	RegisterUserEvent();
}
void Input::RegisterUserEvent(){
	Uint32 myEventType = SDL_RegisterEvents(1);
	if (myEventType != ((Uint32)-1)) {
	    userEvt.type = myEventType;
	    userEvt.user.code = 0;
	    userEvt.user.data1 = NULL;
	    userEvt.user.data2 = NULL;
	}
}
void Input::PushUserEvent(){
	SDL_PushEvent(&userEvt);
}
bool Input::IsUserEvent(){
	return (evt.type == SDL_USEREVENT);
}

void Input::Update()
{
	while(SDL_PollEvent(&evt));
}

bool Input::IsMouseButtonDown(byte key)
{
	if(evt.type == SDL_MOUSEBUTTONDOWN)
		if(evt.button.button == key)
			return true;
	return false;
}

bool Input::IsMouseButtonUp(byte key)
{
	if(evt.type == SDL_MOUSEBUTTONUP)
		if(evt.button.button == key)
			return true;
	return false;
}

POINT Input::GetButtonDownCoords()
{
	POINT point;
	point.x = evt.button.x;
	point.y = evt.button.y;

	return point;
}

bool Input::IsKeyDown(byte key)
{
	return (evt.type == SDL_KEYDOWN && evt.key.keysym.sym == key);
}

bool Input::IsKeyUp(byte key)
{
	return (evt.type == SDL_KEYUP && evt.key.keysym.sym == key);
}

byte Input::GetPressedKey()
{
	return evt.key.keysym.sym;
}

bool Input::IsExit()
{
	return (evt.type == SDL_QUIT || (evt.type == SDL_WINDOWEVENT ? evt.window.event==SDL_WINDOWEVENT_CLOSE : 0) );
}