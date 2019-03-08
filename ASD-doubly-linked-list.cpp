// Implementa le funzioni sulle liste realizzate con strutture doppiamente collegate, circolari, con sentinella, senza ordinamento

#include "ASD-doubly-linked-list.h"


using namespace list;

struct list::node {  // descrivo qui, fuori dal namespace, come e' fatta la struttura node: i dettagli interni non saranno visibili dal main
Elem info;
node *prev;
node *next;
};

/*************************************************/
/* Implementazione delle operazioni di utilita'� */
/*************************************************/

void readFromStream(istream& str, List& l)
{
  createEmpty(l);
  Elem e;
  str>>e;
  if (!str) throw runtime_error("Errore inserimento dati\n");
  while (e != FINEINPUT)  // assumiamo che il segnale di fine input nel file sia  FINEINPUT
  {     
    addRear(e, l);   // aggiunge l'elemento alla fine della lista
    str>>e;
    if (!str) throw runtime_error("Errore inserimento dati\n");
  }
}

void readFromFile(string nome_file, List& l)  /* legge il contenuto di una lista da file */
{
  ifstream ifs(nome_file.c_str()); // apertura di uno stream associato ad un file, in lettura
  readFromStream(ifs, l);
}



void readFromStdin(List& l)         /* legge il contenuto di una lista da standard input */
{
  cout << "\nInserire una sequenza di numeri separati da spazi seguiti da " << FINEINPUT << " per terminare\n";
  readFromStream((std::cin), l);
}


void print(const List& l)           /* stampa la lista */
{
   List q = l->next;  // q "salta la sentinella" e va alla testa della lista
   while (q != l)     // avanza finche' non finisce la lista
    {
      cout << q->info << "; ";
      q = q->next; 
    }
}

/**************************************************/
/* Implementazione delle operazioni nel namespace */
/**************************************************/

node* getNodeAt (int pos, const List& l) {
  node* currentNode = l->next; // Salta la sentinella

  int len = 0; // Inizializza la lunghezza
  while (true) { // Finche non arriva alla fine
    if (currentNode->next == l) // Se il prossimo nodo e' la sentinella
      break; // E' arrivato alla fine (interrompe il ciclo)

    if (len == pos) // Se e' arrivato alla posizione corretta
      break; // Interrompe il ciclo
    
    currentNode = currentNode->next; // Seleziona il nodo successivo
    len++; // Conta 1 elemento in piu'
  }

  if (len != pos) // Se il ciclo e' terminato prima di arrivare alla posizone (lista troppo corta)
    return nullptr; // Restituisce un puntatore nullo

  return currentNode; // Restituisce il nodo corrispondente
}


void link (node* left, node* right) { // Crea i collegamenti per left<->right
  left->next = right;
  right->prev = left;
}


void link (node* left, node* center, node* right) { // Crea i collegamenti per left<->center<->right (equivale a un inserimento tra left e right se sono vicini)
  link(left, center); // Ricrea i collegamenti per left<->center
  link(center, right); // Ricrea i collegamenti per center<->right
}


void list::clear(const List& l)                 /* "smantella" la lista svuotandola */
{
  node* currentNode = l->next; // Salta la sentinella

  while (true) { // Finche non arriva in fondo
    node* tmp = currentNode; // Mette da parte il nodo corrente
    currentNode = currentNode->next; // Ottiene il prossimo nodo
    
    delete tmp; // Elimina dalla memoria il nodo precedentemente messo da parte
    
    if (currentNode == l) // Quando il prossimo nodo e' la sentinella
      break; // Interrompe il ciclo (e' arrivato in fondo)
  }

  link(l, l); // "Resetta" la sentinella
}


Elem list::get(int pos, const List& l)        /* restituisce l'elemento in posizione pos se presente; restituisce un elemento che per convenzione si decide che rappresenta l'elemento vuoto altrimenti*/
{
  node* n = getNodeAt(pos, l); // Ottiene il nodo in posizione pos

  if (n != nullptr) // Se il nodo esiste
    return n->info; // Restituisce il suo valore
  
  return EMPTYELEM; // Altriemnti restituisce elemento vuoto
}


void list::set(int pos, Elem e, const List& l)        /* modifica l'elemento in posizione pos, se la posizione e' ammissibile */
{
  node* n = getNodeAt(pos, l); // Ottiene il nodo in posizione pos

  if (n != nullptr) // Se il nodo esiste
    n->info = e; // Imposta il suo valore
}


void list::add(int pos, Elem e, const List& l)        /* inserisce l'elemento in posizione pos, shiftando a destra gli altri elementi */
{
  node* n = getNodeAt(pos, l); // Ottiene il nodo in posizione pos

  node* newNode = new node(); // Inizializza un nuovo nodo
  n->info = e; // Gli assegna l'elemento da aggiungere

  link(n, newNode, n->next); // Ricrea i collegamenti
}


void list::addRear(Elem e,  const List& l)        /* inserisce l'elemento alla fine della lista */
{
  node* sentinel = l; // Ottiene la sentinella
  node* last = sentinel->prev; // Ottiene l'ultimo nodo

  node* newNode = new node(); // Inizializza un nuovo nodo
  newNode->info = e; // Gli assegna l'elemento da aggiungere

  link(last, newNode, sentinel); // Ricrea i collegamenti
}


void list::addFront(Elem e, const List& l)        /* inserisce l'elemento all'inizio della lista */
{
  node* sentinel = l; // Ottiene la sentinella
  node* first = sentinel->next; // Ottiene il primo nodo

  node* n = new node(); // Inizializza un nuovo nodo
  n->info = e; // Gli assegna l'elemento da aggiungere

  link(sentinel, n, first); // Ricrea i collegamenti
}


void list::removePos(int pos, const List& l)      /* cancella l'elemento in posizione pos dalla lista */
{
  node* n = getNodeAt(pos, l); // Ottiene il nodo in posizione pos

  if (n != nullptr) {// Se esiste
    link(n->prev, n->next); // Ricrea i collegamenti tra il precedente e il successivo
    delete n; // Elimina il nodo dalla memoria
  }
}


void list::removeEl(Elem e, const List& l)        /* cancella tutte le occorrenze dell'elemento elem, se presenti, dalla lista */
{
  node* currentNode = l->next; // Salta la sentinella

  while (true) {
    if (currentNode->next == l) // Quando il prossimo nodo e' la sentinella
      break; // E' arrivato alla fine

    node* tmp = currentNode; // Mette da parte il nodo corrente

    currentNode = currentNode->next; // Seleziona il prossimo nodo

    if (tmp->info == e) { // Se il nodo corrente contiene l'elemento e
      link(tmp->prev, tmp->next); // Ricrea i collegamenti tra il precedente e il successivo
      delete tmp; // Elimina il nodo dalla memoria
    }
  }
}


bool list::isEmpty(const List& l)         /* restituisce true se la lista e' vuota (ed e' vuota se il next di l, e' l stessa */
{
  return l == l->next; //true se la sentinella e' il nodo successivo a se stesso
}


int list::size(const List& l)           /* restituisce la dimensione della lista */
{
  if (l->next == l) // Se il nodo successivo alla sentinella e' la sentinella stessa
    return 0; // La lista e' vuota

  node* currentNode = l->next; //Salta la sentinella

  int len = 0; // Inizializza la lunghezza a 0
  while (true) { // Finche non arriva alla fine
    len++; // Conta 1 elemento in piu'

    currentNode = currentNode->next; // Seleziona il prossimo nodo

    if (currentNode == l) // Quando il prossimo nodo e' la sentinella
      break; // E' arrivato alla fine
  }

  return len; // Restituisce la lunghezza
}


void list::createEmpty(List& l)           /* crea la lista vuota */
{
  if (l != nullptr) // Se l non e' un puntatore nullo (quindi probabilmente e' una lista)
    clear(l); // La svuoto (libero la memoria da tutti i nodi che altrimenti rimarrebbero orfani)
  
  l = new node(); // Creo un nuovo nodo che sara' la sentinella
  link(l, l); // Collego la sentinella a se stessa
}
