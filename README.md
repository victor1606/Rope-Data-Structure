# Rope-Data-Structure
Tree data structure which is used to store or manipulate large strings in a 
more efficient manner - Implemented in C

Dima Elena-Madalina & Calugaritoiu Ion-Victor

In cadrul proiectului am implementat functionalitatea unei structuri de date
arborescente numita Rope, aceasta permitand efectuarea unor operatii
(concatenate, search, split, insert, delete) pe siruri de caractere.

	- "concat" - permite unirea a doi arbori de tip Rope: se aloca un nou nod
	radacina; radacinile arborilor primiti ca parametru sunt trecute drept
	copii ai noului nod creat; noua radacina este folosita pentru creearea unui
	nou Rope, folosind functia "makeRopeTree";

	- "indexRope" - returneaza caracterul aflat la indexul "idx", obtinut de
	catre functia "find_index": arborele este parcurs recursiv; cat timp
	variabila "ok" este nula, string-urile sunt parcurse iar variabila "step"
	este incrementata; in momentul in care "step" este egal cu indexul dorit,
	"ok" ia valoarea 1 si caracterul la care a ajuns parcurgerea este returnat;

	- "search" - returneaza string-ul aflat in intervalul primit ca parametru:
	se aloca memorie pentru un nou string; pentru fiecare index din intervalul
	primit, este apelata functia "indexRope", astfel construind sirul de
	caractere dorit;

	- "split" - imparte un arbore de tip rope in 2 subarbori in functie de
	caracterul aflat la pozitia "idx": se obtine nodul care contine litera
	de la indexul dorit folosind functia "find_node";
		- "find_node" - primeste ca parametri: root-ul Rope-ului, index-ul
		caracterului dorit si variabilele "step" si "ok", care se comporta
		asemanator ca in functia "indexRope"; in momentul in care nodul este
		gasit, lui "idx" i se atribuie indexul pe care se afla caracterul
		cautat in node->str;
		- in cazul in care se face split dupa ultimul caracter al sirului, "ok"
	va avea valoarea 0, subarborele stang va fi arborele initial, iar pentru
	cel drept se creeaza un nou RopeTree cu un root care contine sirul
	EMPTY (" ");
		- daca nodul gasit memoreaza un string, acesta se va imparti in doua
	subsiruri: "string1" (contine literele de dinainte de "idx") si "string2"
	(contine literele de dupa "idx"):
			- daca "idx" este 0 (split de la primul caracter al lui node->str),
			"string1" va lua valoarea "\0";
			- daca "idx" este egal cu "length" (split de la ultimul caracter al
			lui node->str), "string2" va lua valoarea "\0";
		- cu cele doua subsiruri se formeaza nodurile "node1" si "node2", care
		sunt atribuite drept radacini pentru noii arbori de tip rope: "tr_left"
		si "tr1".
		- daca nodul gasit contine doar un caracter (length == 1) nu mai este
		necesara impartirea sirului memorat in node si se formeaza direct
		arborele "tr1" cu radacina node;
		- se cauta parintele lui node (find_parent); daca node este copil
		drept, se formeaza "tr_left" cu radacina parent->left;
		- daca parintele lui node este diferit de radacina se verifica daca mai
		are alti copii in afara de node:
			- in cazul in care node este copil stang si parintele mai are si un
		copil drept, se formeaza un arbore auxiliar cu radacina parent->right
		care se adauga subarborelui drept "tr1" prin functia "concat";
			- in cazul in care node este copil drept si parintele are si un
		copil stang, arborele auxiliar format cu radacina parent->left se
		adauga subarborelui stang "tr_left";
		- se parcurge arborele initial de la parintele nodului node pana la
		radacina cautandu-se succesiv nodurile parinte ale nodurilor parinte
		si se fac aceleasi verificari ca pentru parintele nodului node;

	- "insert" - insereaza un string pe pozitia "idx" intr-un RopeTree:
	se efectueaza o operatie de split la indexul dorit; cu cele doua noduri
	rezultate se creeaza doi arbori de tip Rope: "rt1" si "rt2"; se aloca un
	nou nod care contine string-ul dat ca parametru, si este atribuit drept
	root unui nou RopeTree "new_tree"; se efectueaza doua concatenari: intre
	"rt1" si "new_tree", apoi intre arborele rezultat la prima concatenare si
	"rt2";

	- "delete" - sterge substring-ul cuprins in intervalul dat ca parametru:
		- se efectueaza split pe arborele initial la index-ul "start"; subarborele
	stang este stocat in "rt1";
		- se efectueaza split pe arborele initial la index-ul "start + len";
	subarborele drept este stocat in "rt2";
		- "rt1" si "rt2" se concateneaza, rezultand arborele final;
