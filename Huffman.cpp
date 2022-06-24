#include <iostream>
#include <fstream>
#include <queue>
#include <unordered_map>
#include <vector>
using namespace std;

//GESTIONE E ANALISI CARATTERI FILE INPUT
//Singola cella
struct cell{
  int frequenza;
  char carattere;
  cell* next;
};
typedef cell* list;

//Creo nuova cella
cell* new_cell(char carattere){
  cell* temp = new cell;
  temp->frequenza = 1;
  temp->carattere = carattere;
  temp->next = NULL;
  return temp;
}

//Inserimento in testa
list insert_cell(list& mylist, char elem){
  cell* cur = mylist;
  cell* temp = new_cell(elem);
  if(mylist == NULL){
    mylist = temp;
    return mylist;
  }
  while(cur->next != NULL)
    cur = cur->next;
  cur->next = temp;
  return mylist;
}

//Conto elementi lista
int count(list mylist){
  int counter = 0;
  cell* cur = mylist;
  while(cur != NULL){
    counter = counter + cur->frequenza;
    cur = cur->next;
  }
  return counter;
}

//Calcolo probabilità basata su frequenza
float prob(double frequenza, double totale){
  float prob = frequenza/totale;
  return prob;
}

//Stampo vocabolario, con frequenze e probabilità
void print(list mylist){
  cell* cur = mylist;
  int counter = count(mylist);
  while(cur != NULL){
    cout << cur->carattere << " - frequenza: " << cur->frequenza << " " << prob(cur->frequenza, counter) <<  endl;
    cur = cur->next;
  }
}

//L'elemento dato è presente nella list? se si aumento la sua frequenza
bool is_present(list mylist, char carattere){
  cell* aux = mylist;
  while(aux != NULL){
    if(aux->carattere == carattere){
      aux->frequenza++;
      return true;
    }
    aux = aux->next;
  }
  return false;
}

//FUNZIONI HUFFMAN
//Nodo singolo
struct Node{
	char carattere;
	int frequenza;
	Node *left, *right;
};

//Creo nuovo nodo albero
Node* getNode(char carattere, int frequenza, Node* left, Node* right){
	Node* node = new Node();
  node->carattere = carattere;
	node->frequenza = frequenza;
	node->left = left; //Può essere NULL
	node->right = right; //Può essere NULL

	return node;
}

//Confronto elementi
struct comp{
	bool operator()(Node* n1, Node* n2){
		return n1->frequenza > n2->frequenza; //Frequenza più bassa priorità più alta
	}
};

//Codifico secondo HUFFMAN
void encode(Node* root, string code, unordered_map<char, string> &huffmanCode){
	if (root == NULL) return; //Fine albero

  //Nodo foglia
	if (!root->left && !root->right){
		huffmanCode[root->carattere] = code;
	}

	encode(root->left, code + "0", huffmanCode); //A sinistra aggiungo uno 0 alla codifica
	encode(root->right, code + "1", huffmanCode); //Viceversa a destra aggiungo un 1
}

//Costruisco l'albero di HUFFMAN
unordered_map<char, string> buildHuffmanTree(list mylist){
  //Coda di priorità
	priority_queue<Node*, vector<Node*>, comp> coda;

	//aggiungo ogni carattere alla coda
  cell* aux = mylist;
	while(aux != NULL){
		coda.push(getNode(aux->carattere, aux->frequenza, NULL, NULL)); //Sono tutte foglie
    aux = aux->next;
	}

	while (coda.size() != 1){
		Node *left = coda.top(); //Prendo il primo elemento
    coda.pop(); //Rimuovo l'elemento preso in precedenza
		Node *right = coda.top(); //Secondo elemento più prioritario
    coda.pop();

		//Creo un mini albero, che sarà poi un sottoalbero del risultato finale
    //Che avrà come radice un nodo nuovo con frequenza pari alla somma di left+right
		coda.push(getNode('\0', left->frequenza + right->frequenza, left, right));
	}

	//Puntatore a radice dell'Huffman tree
	Node* root = coda.top();
  unordered_map<char, string> huffmanCode;
  encode(root, "", huffmanCode);
  cout << "XXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXX" << endl;
  cout << "XXXXXXXX      Codifica di HUFFMAN      XXXXXXXX" <<endl;
  cout << "XXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXX" << endl << endl;
  cout << "Caratteri:" << endl;
  for (auto pair: huffmanCode) {
  	cout << pair.first << " " << pair.second << '\n';
  }

  return huffmanCode;
}

//MAIN PROGRAM
int main (){
  char next;
  ifstream INPUT("prova.txt");

  list vocabolario = NULL;
  while (INPUT.get(next)){
    if(next == '\n')
      continue;

  if(!is_present(vocabolario, next))
    vocabolario = insert_cell(vocabolario, next);
  }

  unordered_map<char, string> huffmanCode = buildHuffmanTree(vocabolario);
  string code;
  ifstream reRead("prova.txt");
  while (reRead.get(next)){
    if(next == '\n')
      continue;
    code = code + huffmanCode[next];
  }
  //cout << "\nEcco il testo codificato:\n" << code << endl; //LINEA DI CODICE PER STAMPARE LA CODIFICA DEL TESTO
}
