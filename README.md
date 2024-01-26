![unit-tests](https://github.com/ChristophervonKlitzing/PolyLinkRPC/actions/workflows/run-tests.yml/badge.svg?branch=main)
[![codecov](https://codecov.io/github/ChristophervonKlitzing/PolyLinkRPC/graph/badge.svg?token=YY1PC4OVSO)](https://codecov.io/github/ChristophervonKlitzing/PolyLinkRPC)

# PolyLinkRPC
Flexible language agnostic and connection-based RPC implementation with bindings for different programming languages.

# Planned Features
- Type independent library core usable in other programming languages (with bindings)
- Use of network byte order ⟶ system independent
- Connection based (e.g. TCP)
- Types of Remote Procedure Calls calls
  * **Synchronous:** wait for the returned value and return it
  * **Asynchronous:** do not wait but instead return a Future object
  * **No-Return:** No return-value will be sent (more efficient if just `NULL` is returned)
- Controlled exposure of callable functions

# Motivation
This is for fun and might or might not result in a working library. It is about getting familiar with GitHub and GitHub actions
as well as getting into C++ (a second time).
