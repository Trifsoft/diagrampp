# cpp convetions
- camel case notacija za imena klasa, snake notacija za sve ostalo
- interfejs treba biti prefiksovan sa I
- privatna polja i metodi se prefiksuju sa m_
- pisanje constructor = default/delete, ako ne zelimo/ne zelimo neki konstruktor
- pisanje destructor = default, ako ne pisemo destruktor ali eksplicitno zelimo da  kazemo da nije potrebna implementacija custom destructor-a


# validatori:
- ispitivanje validnih odnosa, asocijacija, nasledjivanje, implementacija, nema smisla svaki odnos za bilo koja dva objekta dijagrama
- problem dijamanta, ciklicnost (kosaraju, komponente povezanosti)
- ako klasa ima dinamicki alocirana stanja mora imati virtuelni destructor, ili ako ima decu, zbog nasledjene dece

# C++ conventions
- CamelCase notation for class names, snake_case notation for everything else
- Interfaces should be prefixed with `I`
- Private fields and methods are prefixed with `m_`
- Use `constructor = default/delete` when we want or do not want a specific constructor
- Use `destructor = default` when we do not implement a destructor but explicitly want to state that no custom destructor implementation is required


# validators:
- Validation of valid relationships: associations, inheritance, implementation; not every relationship makes sense between any two objects in a diagram
- Diamond problem, cyclic dependencies (solutions: connected components indexes)
- If a class has dynamically allocated state, it must have a virtual destructor, or if it has derived classes, due to inheritance


