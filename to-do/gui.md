# gui
- left floating menu
- top bar
- whiteboard (infinite)


# left floating menu
- add object
        dropdown selector of objects

# top bar
- save as
        png, jpg
        import/export as json

# whiteboard
- fill full size
- maybe it should have some squares like paper


# flow example
- user selects object : class, interface, enum...
- object pops up on the whiteboard
- on double tap, options are showed
        options:
        - connect (uml relations, it would be convenient to show just relations that are relative for the object type)
        - remove
        
- e.g. connect is clicked -> derive -> click on other object on the whiteboard -> structure validation -> connect objects

# miscellaneous (don't read)
- every drawed object should show uml relations on right click, or double left click whatever, and option for its removal 
- dynamically every action that is not valid will be dropped, for instance: interface should not derive anything or interface cannot implement interface event though that is possible in cpp it breaks the structure and system design
