#include "gfalt_params.h"


void event_callback_wrapper(const gfalt_event_t e, gpointer user_data)
{
    CallbackObjs* callbacks = static_cast<CallbackObjs*>(user_data);
    if (callbacks->event_callback) {
        boost::python::call<void>(callbacks->event_callback.ptr(), Gfalt_event(e));
    }
}
