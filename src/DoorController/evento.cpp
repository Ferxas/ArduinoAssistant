// #include <functional>
// #include <vector>
// using namespace std;
// class Event
// {
// public:
//     void operator+=(std::function<void()> handler)
//     {
//         handlers_.push_back(handler);
//     }

//     void operator-=(std::function<void()> handler)
//     {
//         handlers_.erase(std::remove(handlers_.begin(), handlers_.end(), handler), handlers_.end());
      
//     }

//     void operator()()
//     {
//         for (auto &handler : handlers_)
//         {
//             handler();
//         }
//     }

// private:
//     std::vector<std::function<void()>> handlers_;
// };