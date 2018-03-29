This document is for those who wants to contribute.

# Code structure

The code of this driver is splited by 3 parts:

- Common
- LowLevel
- HighLevel

LowLevel can include headers in Common, HighLevel can include headers in both Common and LowLevel.

# TODO: provide more sections

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

