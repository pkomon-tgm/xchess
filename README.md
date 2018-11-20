# xchess
(extensible) implementation of chess in c++ using boost.

## Summary
xchess is an extensible c++-implementation of the board game chess - you may already have heard of it, it is sort of popular. 
While it may not be the most optimized nor simplest implementation, it follows a natural design, keeping things as general 
and extensible as possible. Also it is possible to implement other games (with boards similar to chess, checkers for example).

## Build instructions
TODO use fancy build tool such as cmake; for now it has to be compiled by hand

TODO setup build automation

## Usage
TODO write this section

## Status
As it is for now, quite a few things need reengineering and generalization, but the game itself works 
and (as proof-of-concept) a CLI interface - with colored output, maybe does not work on Windows lol) is included. 
Still there are a few features that will be added in near future:

* Stalemate
* Serialization (for save states) into files
* Timers
* Riddles

## Dependencies

* boost (tested with 1.66), libs `chrono`, `serialization`, `wserialization`, `filesystem`, `system`, `thread`,`timer` required (actually dunno, just saw them in my build from ages ago)
