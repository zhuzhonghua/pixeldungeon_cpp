#include "keys.h"

Signal<Keys::Key> Keys::event(true);

void Keys::processTouchEvents(std::vector<SDL_Event>& events)
{
	/*int size = events.size();
	for (int i = 0; i < size; i++) {

		KeyEvent e = events.get(i);

		switch (e.getAction()) {
		case KeyEvent.ACTION_DOWN:
			event.dispatch(new Key(e.getKeyCode(), true));
			break;
		case KeyEvent.ACTION_UP:
			event.dispatch(new Key(e.getKeyCode(), false));
			break;
		}
	}*/
}