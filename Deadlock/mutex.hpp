/*
* Copyright (C) 2018 poppinzhang.
*
* Written by poppinzhang with C++11 <poppinzhang@tencent.com>
*
* This file is head file of the mutex.
* You must include this file if you want to use this mutex.
*
* You should have received a copy of the GNU Lesser General Public
* License along with this file; if not, as you like...
*/

#ifndef DEADLOCK_MUTEX_H
#define DEADLOCK_MUTEX_H

#include <map>
#include <memory>
#include <mutex>
#include <thread>
#include <set>

namespace poppin
{
	class Mutex {
		typedef uint64_t LockId;
	public:
		Mutex() {
			std::lock_guard<std::mutex> lock(id_mutex_);
			if (!recover_id_.empty()) {
				lockid_ = *recover_id_.begin();
				recover_id_.erase(lockid_);
			} else {
				lockid_ = cur_id_++;
			}
		}
		Mutex(const Mutex&) = delete;
		Mutex& operator= (const Mutex&) = delete;
		Mutex(const Mutex&&);
		void operator= (const Mutex&&);
		~Mutex() {
			std::lock_guard<std::mutex> lock(id_mutex_);
			if (!mutex_.try_lock()) {
				mutex_.unlock();
			}
			recover_id_.insert(lockid_);
		}
		//带有死锁检测，如果发现有死锁出现，返回失败
		bool lock() { mutex_.lock(); }
		void unlock() { mutex_.unlock(); }
	private:
		LockId lockid_;
		std::mutex mutex_;

		static LockId cur_id_;
		static std::set<LockId> recover_id_;
		static std::mutex id_mutex_;
		static std::mutex map_mutex_;
		// The lock is used by the thread
		static std::map<LockId, std::thread::id> lock_used_map;
		// The thread is waiting for the lock
		static std::map<std::thread::id, LockId> lock_wait_map_;
	};

	Mutex::LockId Mutex::cur_id_ = 0;
} // namespace poppin

#endif // !DEADLOCK_MUTEX_H