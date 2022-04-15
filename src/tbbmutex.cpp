#include <boost/thread/thread.hpp>
#include <tbb/spin_mutex.h>

#define PICOBENCH_IMPLEMENT_WITH_MAIN
#include <picobench/picobench.hpp>

const int total_count=100000000;
int counter =0;
tbb::spin_mutex m;

void increment_count(int thread_count) {
  for (int i=0; i<total_count/thread_count;i++) {
    tbb::spin_mutex::scoped_lock lock(m);
    counter++;
  }
}

void bench(picobench::state& s)
{
  unsigned int parallism = s.iterations();
  boost::thread_group threads;
  {
    tbb::spin_mutex::scoped_lock lock(m);
    for (unsigned int i=0;i<parallism;i++) {
      boost::thread* th = new boost::thread(&increment_count, parallism);
      threads.add_thread(th);
    }
  }

  picobench::scope time(s);
  threads.join_all();
}

PICOBENCH(bench);

