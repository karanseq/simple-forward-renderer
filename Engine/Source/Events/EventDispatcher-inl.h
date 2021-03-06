#include "EventDispatcher.h"

// Engine includes
#include "Logger/Logger.h"

namespace engine {
namespace events {

template<typename T>
inline EventReceipt EventDispatcher<T>::AddListener(const CallbackType<T>& i_callback)
{
    id_counter_ = id_counter_ == std::numeric_limits<size_t>::max() ? 1 : id_counter_ + 1;
    EventReceipt receipt(T::GetEventID(), id_counter_);
    listeners_.insert(std::make_pair(receipt, i_callback));
    return receipt;
}

template<typename T>
inline void EventDispatcher<T>::RemoveListener(EventReceipt& i_event_receipt)
{
    const auto& it = listeners_.find(i_event_receipt);
    if (it != listeners_.end())
    {
        listeners_.erase(i_event_receipt);
        i_event_receipt.Invalidate();
    }
    else
    {
        LOG_ERROR("%s couldn't find listener associated with receipt!", __FUNCTION__);
    }
}

template<typename T>
inline void EventDispatcher<T>::DispatchEvent(const T& i_event)
{
    for (const auto& listener : listeners_)
    {
        listener.second(i_event);
    }
}

} // namespace events
} // namespace engine