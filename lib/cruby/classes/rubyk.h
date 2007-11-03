#ifndef _RUBYK_H_
#define _RUBYK_H_
#include "node.h"
#include "link.h"
#include "event.h"
#include <list>
#include <sys/timeb.h> // ftime

// 50 ms wait
#define SLEEP_MS 50

class Rubyk
{
public:
  Rubyk();
  
  virtual ~Rubyk();
  
  Node * create_instance(const std::string& pVariable, const std::string& pClass, const Params& pParams);
  
  void create_link(const std::string& pFrom, unsigned int pFromPort, unsigned int pToPort, const std::string& pTo);
  
  Node * get_instance(const std::string& pVariable);
  
  /** Close the door and leave... */
  void quit()
  { mQuit = true; }
  
  /** Main loop. Returns false when rubyk has to quit. */
  bool run();
  
  /** Add an event to the event queue. The server is responsible for deleting the event. */
  void register_event(BaseEvent * pEvent)
  { mEventList.push(pEvent); }
  
  unsigned long mCurrentTime; /**< Current logical time in [ms] since reference. */

  /** Get current real time in [ms] since reference. */
  unsigned long real_time()
  {
    struct timeb t;
    ftime(&t);
    return ((t.time - mTimeRef.time) * 1000) + t.millitm - mTimeRef.millitm;
  }
  
  
private:
  
  /** Try to create links. */
  void create_pending_links();
  
  /** Trigger events with a time older or equal to the current time. */
  void pop_events();
  
  /** Trigger loop events. These are typically the IO 'read/write' of the IO nodes. */
  void trigger_loop_events();
  
  bool mQuit;
  /** Reference to the objects in the system. */
  Hash<std::string, Node *> mInstances;
  
  /** List of pending connections waiting for variable assignements. */
  std::list<Link> mPendingLinks;
  //std::list<Link> mLinksSet;
  
  /** Events ! */
  OrderedList<BaseEvent> mEventList; /**< Ordered event list. */
  
  /** Time reference. All times are [ms] from this reference. */
  struct timeb mTimeRef;
};

#endif