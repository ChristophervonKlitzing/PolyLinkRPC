# Features
Done: Implement NetworkStream and ByteBuffer and serialize task datagram using this instead
TODO: Implement TaskSender and TaskReceiver
The TaskSender and TaskReceiver should implement a version-check which checks if incoming 
datagrams are compatible. Add dedicated version-compatibility check function for that.

TODO: Test TaskSender and TaskReceiver together using a dummy connection
TODO: Add python bindings and update C++ API if necessary to obtain language independency.

TODO: Add start-up protocol which allows exposing functions and checks version compatibility (maybe remove version from Task datagram).
Example: EXPOSE func_1, func_2, add_* (regexes are possible for languages with reflection like python)

TODO: Add implementation for local communication which skips the serialization, sending and de-serialization part.
This makes it possible to run tasks locally or remote using the same API (but on different TaskSender instances).

# CI/CD
TODO: Split test workflow into three workflows and add badges individually to the README in a table
Example:
Platforms | Unit-Tests | C++ build | Python build | Coverage
------------------------------------------------------------
Windows   | passing    |   built   |              |
Linux     | passing    |   built   |              |   48%
macOS     | passing    |   built   |              |

TODO: Add test-coverage and add badge for it to the README

# Documentation
TODO: Add documentation, demos which only apply the core functionality and use-case examples.
TODO: Add use-case example which runs plugins written in different programming languages

TODO: Automate github wiki repo using a workflow in the main repo
https://medium.com/@thecybermonk/the-secret-github-wiki-repo-and-ci-cd-for-docs-fafe2583b72e

# Contribution
TODO: Look into how GitHub contribution works and setup everything for it

# Release
TODO: Add CI/CD for deployment (C++ and Python for now) and create git tag