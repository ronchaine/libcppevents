libcppevents - a generic event system for cpp
=============================================

Introduction
------------
There are multiple sources your software might want to get
events from.  This includes timers, network sockets, filesystem
changes, input events, OS signals, window events, etc.

Basically, you often would benefit from having unified access
to all of these events from single `wait()` or `poll()` call,
and handle those with a single interface.

There are C libraries that handle some of this, but I wanted
to make all-around solution that uses modern C++

tldr: epoll / timerfd / eventfd / inotify / kqueue for cpp

Project scope
-------------
What we want to do is
- Implement an event API
- Make it possible for user to easily add their own event types
- Make it simple to convert a system-specific interface to its
  cppevents equivalent (e.g. tie a BSD socket to the event system)
- Implement simple interfaces to events such as filesystem watches
  and timers
- Make it easy to tie library (e.g. SDL or GLFW) window events to
  the event system
- Support platforms other than Linux, notably Windows, OS X, *BSD
  and freeRTOS
  
What we do not want to do
- Implement entire network API for getting socket, we just handle
  the sockets that are given us from someplace else
- Implement entire graphics API for getting windowing events

OS specifics
------------
The API is based on features available on Linux, mostly because
of the familiarity.  Features supported by both Linux `*fds`,
`inotify`, `epoll` and BSD `kqueue` are the priority.

Help is wanted, especially with Windows side of things, as PO is
not familiar with Windows interfaces.

Current status
--------------

- Event API implemented and workign
- It is simple to add new event types
- SDL2 tie-ins exist

