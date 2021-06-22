#include <memory>

template<typename T>
class Recycler {
public:
    static auto getInstance () -> std::shared_ptr<Recycler<T>>;

private:
    static std::shared_ptr<Recycler<T>> _instance;
};

template<typename T>
std::shared_ptr<Recycler<T>> Recycler<T>::_instance{nullptr}; 

template<typename T>
inline auto Recycler<T>::getInstance () -> std::shared_ptr<Recycler<T>> {
    if (!_instance)
        _instance = std::make_shared<Recycler<T>>();
    
    return _instance;
}