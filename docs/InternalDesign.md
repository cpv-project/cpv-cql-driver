# Internal Design

This document is for those who wants to contribute.

## Code structure

Code of this driver are splited as 3 parts:

- Common: contains common types and functions used from LowLevel and HighLevel
- LowLevel: contains internal types which can't access from public header files under include folder
- HighLevel: contains public types which can access from public headers files under include folder

LowLevel can include headers from Common, HighLevel can include headers from both Common and LowLevel.

## TODO: provide more sections

- Define new column type
- Define new logger
- Define new node collection
- Define new authenticator
- Define new compressor
- Define new connector
- Define new column types
- Define new request message
- Define new response messages
- Connection stream should keep alive until sendMessage or waitNextMessage is done

