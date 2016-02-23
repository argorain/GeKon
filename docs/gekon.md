# Zadání

Vstupem obrázky a konvoluční matice, kterýma upravíme ty obrázky nebo rovnou upravené obrázky?

# Nápady na řešení

Postup GA:
* init generace konvolučních matic (musíme mít nějaké omezení na jejich hodnoty a velikost)
loop:
* fitness - kvadrát odchylky?
* elitismus
* křížení - průměrování? Výběr náhodných prvku z jedné a z druhé matice?
* mutace - náhodné změny prvků masky?
* selekce

Otázky k dořešení:
* Co je genom? Vektor popisující matici(rozměry, průměr, apod.) nebo linearizovaná matice(řádky za sebou v jednom řádku)?
* Co je fitness? Kvadrát odchylek?
* Co bude mutace/křížení znamenat u konvolučních matic případně příznakových vektorů?

Architektura programu:
Meta: C++14, smart_ptr, etc. FTW! Prostě využít nového C++ :-)

vst. obr = const vektor matic
generace = const vektor matic
fitness = vektor skalárů

výpočet fitness = n vláken počítá konvoluce a fitnessy (thread pool?)
nová generace ???

