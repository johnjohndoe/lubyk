#include "rubyk.h"
#include "node.h"
#include "slot.h"

Rubyk::~Rubyk()
{
  std::vector<std::string>::iterator it;
  std::vector<std::string>::iterator end = mInstances.end();
  Node ** node;
  
  for(it = mInstances.begin(); it < end; it++) {
    if (node = mInstances.get(*it))
      delete *node; // destroy node
  }
}

Node * Rubyk::create_instance (const std::string& pVariable, const std::string& pClass, const Params& pParams)
{
  Node * node = Node::create(pClass, pParams);
  Node ** previous;
  std::string varName;

  if (node) {
    if (pVariable != "")
      node->set_variable_name(pVariable);
    
    previous = mInstances.get(node->variable_name());
    
    if (previous)
      delete *previous; // kill the node pointed by variable name
      
    mInstances.set(node->variable_name(), node);
    create_pending_links();
  }
  return node;
}


void Rubyk::create_link(const std::string& pFrom, unsigned int pFromPort, unsigned int pToPort, const std::string& pTo)
{
  mPendingLinks.push_back(Link(pFrom,pFromPort,pToPort,pTo));
  create_pending_links();
}

// FIXME: on node deletion/replacement, remove all pending links related to this node ?.
void Rubyk::create_pending_links()
{
  std::list<Link>::iterator it,end;
  Node ** fromNode, ** toNode;
  Slot * fromPort, * toPort;
  end = mPendingLinks.end();
  it=mPendingLinks.begin();
  while(it != end) {
    if ((fromNode = mInstances.get(it->mFrom)) && (toNode = mInstances.get(it->mTo))) {
      if ((fromPort = (*fromNode)->outlet(it->mFromPort)) && (toPort = (*toNode)->inlet(it->mToPort))) {
        fromPort->connect(toPort);
        // remove from pending links
        it = mPendingLinks.erase(it);
      } else {
        // bad ports (we could remove from list...)
        it++;
      }
    } else {
      // instances not created yet
      it++;
    }
  }
}

Node * Rubyk::get_instance(const std::string& pVariable)
{
  Node ** node = mInstances.get(pVariable);
  if (node)
    return *node;
  else
    return NULL;
}

bool Rubyk::run()
{
  sleep(0.3);
  return !mQuit;
}