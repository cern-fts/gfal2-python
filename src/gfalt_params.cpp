#include "gfalt_params.h"


void event_callback_wrapper(const gfalt_event_t e, gpointer user_data)
{
	GfalPy::scopedGILLocker lock;
    CallbackObjs* callbacks = static_cast<CallbackObjs*>(user_data);
    if (callbacks->event_callback) {
        boost::python::call<void>(callbacks->event_callback.ptr(), Gfalt_event(e));
    }
}



void monitor_callback_wrapper(gfalt_transfer_status_t h, const char* src, const char* dst, gpointer user_data)
{
	GfalPy::scopedGILLocker lock;	
    CallbackObjs* callbacks = static_cast<CallbackObjs*>(user_data);
    if (callbacks->monitor_callback) {
        size_t avg     = gfalt_copy_get_average_baudrate(h, NULL);
        size_t inst    = gfalt_copy_get_instant_baudrate(h, NULL);
        size_t trans   = gfalt_copy_get_bytes_transfered(h, NULL);
        time_t elapsed = gfalt_copy_get_elapsed_time(h, NULL);

        boost::python::call<void>(callbacks->monitor_callback.ptr(), src, dst, avg, inst, trans, elapsed);
    }
}
