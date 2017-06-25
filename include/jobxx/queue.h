// jobxx - C++ lightweight task library.
//
// This is free and unencumbered software released into the public domain.
// 
// Anyone is free to copy, modify, publish, use, compile, sell, or
// distribute this software, either in source code form or as a compiled
// binary, for any purpose, commercial or non - commercial, and by any
// means.
// 
// In jurisdictions that recognize copyright laws, the author or authors
// of this software dedicate any and all copyright interest in the
// software to the public domain. We make this dedication for the benefit
// of the public at large and to the detriment of our heirs and
// successors. We intend this dedication to be an overt act of
// relinquishment in perpetuity of all present and future rights to this
// software under copyright law.
// 
// THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND,
// EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF
// MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT.
// IN NO EVENT SHALL THE AUTHORS BE LIABLE FOR ANY CLAIM, DAMAGES OR
// OTHER LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE,
// ARISING FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR
// OTHER DEALINGS IN THE SOFTWARE.
// 
// For more information, please refer to <http://unlicense.org/>
//
// Authors:
//   Sean Middleditch <sean.middleditch@gmail.com>

#if !defined(_guard_JOBXX_QUEUE_H)
#define _guard_JOBXX_QUEUE_H
#pragma once

#include "delegate.h"
#include "job.h"
#include "context.h"
#include <utility>

namespace jobxx
{

    namespace _detail { struct task; }

    class queue
    {
    public:
        queue();
        ~queue();

        queue(queue const&) = delete;
        queue& operator=(queue const&) = delete;

        template <typename InitFunctionT> job create_job(InitFunctionT&& initializer);
        template <typename FunctionT> void spawn_task(FunctionT&& work);

        void wait_job_actively(job const& awaited);

        bool work_one();
        void work_all();

    private:
        struct impl;

        void _spawn_task(delegate work, _detail::job* parent);
        void _execute(_detail::task& item);

        impl* _impl = nullptr;

		friend context; // to call _spawn_task
    };

    template <typename InitFunctionT>
    job queue::create_job(InitFunctionT&& work)
    {
		job group;
		context ctx(*group._get_impl(), *this);
		work(ctx);
		return group;
    }
	
    template <typename FunctionT>
    void queue::spawn_task(FunctionT&& work)
    {
        _spawn_task(std::forward<FunctionT>(work), nullptr);
    }

}

#endif // defined(_guard_JOBXX_QUEUE_H)
