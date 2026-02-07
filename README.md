# Diagram++

## Kratki opis

Diagram++ je alat za vizuelizaciju C++ klasa i njihovih medjusobnih odnosa. Projekat omogucava pregled i kreiranje klasa, njihovih metoda, polja, konstruktora, kao i hijerarhije nasledjivanja kroz interaktivni graficki prikaz. Cilj je olaksati razumevanje strukture programa, analiziranje arhitekture i pracenje OOP odnosa u C++ projektima na intuitivan i vizualno pregledan nacin.

## Pokretanje projekta

```bash
# Kreiranje build direktorijuma i konfiguracija
cmake -S . -B build

# Bildovanje projekta
cmake --build build

# Pokretanje aplikacije
./build/DiagramPP

# Pokretanje testova
./build/DiagramTest
```

## Koriscenje

### Pocetni ekran

Prilikom pokretanja aplikacije, prikazuje se glavni meni sa dve opcije:

- **New Project** - kreiranje novog dijagrama od nule
- **Import Project** - ucitavanje postojeceg dijagrama iz JSON fajla

### Rad sa dijagramom

#### Dodavanje klasa i struktura

Na bocnom meniju nalaze se dugmici za dodavanje novih elemenata na dijagram. Klikom na **Class** ili **Struct** otvara se dijalog za unos imena, nakon cega se element pojavljuje na platnu.

#### Uredjivanje sadrzaja klase

Svaka klasa na dijagramu prikazuje:

- Ime klase
- Polja (sa tipovima i vidljivoscu)
- Metode (sa potpunim potpisima)

Klikom na **+** dugme unutar klase dodaju se nova polja i metode. Dvoklikom na postojece elemente moguce je njihovo uredjivanje.

#### Povezivanje klasa

Aplikacija podrzava sest tipova UML veza:

| Veza | Opis |
|------|------|
| Inheritance | Nasledjivanje izmedju klasa |
| Association | Asocijacija (opsta veza) |
| Realization | Realizacija interfejsa |
| Aggregation | Agregacija (slaba celina-deo veza) |
| Composition | Kompozicija (jaka celina-deo veza) |
| Dependency | Zavisnost izmedju klasa |

Za kreiranje veze potrebno je:

1. Izabrati tip veze u bocnom meniju
2. Aktivirati **Linkage Mode**
3. Kliknuti na prvu klasu, pa na drugu klasu

#### Brisanje elemenata

1. Aktivirati **Remove Mode**
2. Klik na klasu - brise klasu sa dijagrama
3. Klik na dve klase - brise vezu izmedju njih

### Undo / Redo

Sve akcije na dijagramu mogu se ponistiti i ponoviti:

- **Undo**: `Ctrl+Z`
- **Redo**: `Ctrl+Shift+Z`

### Eksportovanje

Dijagram se moze eksportovati na tri nacina:

- **Export PNG** - cuva vizuelni prikaz dijagrama kao sliku
- **Export JSON** - cuva strukturu dijagrama za kasniji uvoz i dalji rad
- **Generate Code** - generise kompletan C++ projekat sa `.hpp` i `.cpp` fajlovima, ukljucujuci i `CMakeLists.txt` za bildovanje generisanog koda

Prilikom generisanja koda moguce je izabrati konvenciju imenovanja fajlova (`snake_case` ili `CamelCase`).

## Demo snimak

Demonstraciju korišćenja projekta možete pronaći na [sledećem linku](https://youtu.be/twqH1DRyIRE)

## Clanovi tima

- [Nikola Trifunovic 13/2022](https://gitlab.com/trifsoft)
- [David Curuvija 19/2022](https://gitlab.com/davidcuruvija)
- [Filip Djurkovic 158/2022](https://gitlab.com/Itra7)
- [Marko Perisic 307/2023](https://gitlab.com/perisicMarko)
- [Dobrivoje Vujovic 51/2022](https://gitlab.com/DobrivojeVujovic)
