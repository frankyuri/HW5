#include <iostream>
#include <algorithm>
#include <math.h>
using namespace std;
#define MAX_TERMS 10
class PolynomialTerm {
public:
	int coef;
	int expo;
};                                                          

class ArrayPolynomial {                                     
public:                                                     
	PolynomialTerm terms[MAX_TERMS]; 

	void clear() 
	{
		for (int i = 0; i < MAX_TERMS; i++) {
			terms[i].coef = 0;
			terms[i].expo = 0;
		}

		return;
	}

	void inputTerms(int coef, int expo) 
	{
		for (int i = 0; i < MAX_TERMS; i++)
		{
			if (expo == terms[i].expo) {
				if (coef) {
					terms[i].coef = coef;
					break;
				}
				for (int q = 0; q < MAX_TERMS-1; q++)
					terms[q] = terms[q + 1];
				break;
			}
			if (expo > terms[i].expo && coef) {
				for (int w = MAX_TERMS; w >= i; w--) {
					terms[w] = terms[w - 1];
				}
				terms[i].expo = expo;
				terms[i].coef = coef;
				break;
			}
		}
		return;
	}

	void addArrayBasedPoly(ArrayPolynomial& a, ArrayPolynomial& b) 
	{
		int aa = 0, bb = 0, dd = 0;
		while (1)
		{
			if (a.terms[aa].expo == b.terms[bb].expo) {
				terms[dd].expo = a.terms[aa].expo;
				terms[dd].coef = a.terms[aa].coef + b.terms[bb].coef;
				if (terms[dd].coef == 0)
					dd--;
				aa++, bb++, dd++;
			}
			else if (a.terms[aa].expo > b.terms[bb].expo)
			{
				terms[dd].expo = a.terms[aa].expo;
				terms[dd].coef += a.terms[aa].coef;
				aa++, dd++;
			}
			else
			{
				terms[dd].expo = b.terms[bb].expo;
				terms[dd].coef += b.terms[bb].coef;
				dd++, bb++;
			}
			if ((a.terms[aa].coef == 0 && a.terms[aa].expo == 0 && b.terms[bb].coef == 0 && b.terms[bb].expo == 0))
				break;
			if (aa == MAX_TERMS + 1)
				break;
			if (bb == MAX_TERMS + 1)
				break;
		}
		return;
	}

	void reverseArrayBasedPoly() 
	{
		int i,k;
		ArrayPolynomial tmp;
		for ( i = 0; i < MAX_TERMS; i++)
			if (terms[i].expo == 0 && terms[i].coef ==0) break;
		for ( k = 0; k < i; k++)
		{
			tmp.terms[i - 1 - k].expo = terms[k].expo;
			tmp.terms[i - 1 - k].coef = terms[k].coef;
		}
		for (int j = 0; j < i; j++)
		{
			terms[j].expo = tmp.terms[j].expo;
			terms[j].coef = tmp.terms[j].coef;
		}
		return;
	}

	void printArrayBasedPoly() 
	{
		if (terms[0].coef == 0)
			return;

		if (terms[0].expo == 0)
			std::cout << terms[0].coef;
		else
			std::cout << terms[0].coef << "X^" << terms[0].expo;

		for (int i = 1; i < MAX_TERMS; i++) {
			if (terms[i].coef == 0)
				return;

			if (terms[i].expo == 0)
				std::cout << " + " << terms[i].coef;
			else
				std::cout << " + " << terms[i].coef << "X^" << terms[i].expo;
		}

		return;
	}

	int compute(int x)
	{
		int all = 0;
		for (int i = 0; terms[i].coef != 0; i++) 
			all += terms[i].coef * pow(x, terms[i].expo);
		return all;
	}
};

class LinkedPolynomialTerm {
public:
	int coef;
	int expo;
	LinkedPolynomialTerm* nextTermPtr; 
};                                                                  

class LinkPolynomial {                               
public:                                                         
	LinkedPolynomialTerm* polynomialTermPtr = nullptr; 

	void clear() 
	{
		LinkedPolynomialTerm* tmpPtr;

		while (polynomialTermPtr != nullptr) {
			tmpPtr = polynomialTermPtr;
			polynomialTermPtr = polynomialTermPtr->nextTermPtr;
			delete tmpPtr;
		}

		return;
	}

	void inputLinkTerms(int coef, int expo) 
	{
		LinkedPolynomialTerm* tmpPtr;

		tmpPtr = new LinkedPolynomialTerm;
		tmpPtr->coef = coef;
		tmpPtr->expo = expo;
		
		if (polynomialTermPtr == 0|| polynomialTermPtr->expo <= expo)
		{
			if (polynomialTermPtr  && polynomialTermPtr->expo == expo) {
				if (coef != 0)
					tmpPtr->nextTermPtr = polynomialTermPtr->nextTermPtr;
				else
					tmpPtr = polynomialTermPtr->nextTermPtr;
			}
			else if (coef == 0)
				return;
			else
				tmpPtr->nextTermPtr = polynomialTermPtr;
			polynomialTermPtr = tmpPtr;
		}
		else
		{
			LinkedPolynomialTerm* tmp = polynomialTermPtr;
			while (true) {
				if (tmp->nextTermPtr && tmp->nextTermPtr->expo <= expo){
					if (tmp->nextTermPtr->expo == expo) {
						if (coef) 
							tmp->nextTermPtr = tmp->nextTermPtr->nextTermPtr;
						else
							tmpPtr = tmp->nextTermPtr->nextTermPtr;
					}
					else if (!coef) 
						break;
					else
						tmpPtr->nextTermPtr = tmp->nextTermPtr;
					tmp->nextTermPtr = tmpPtr;
					break;
				}
				else if (tmp->nextTermPtr==0) {
					if (coef != 0) {
						tmp->nextTermPtr = tmpPtr;
						tmpPtr->nextTermPtr = nullptr;
					}
					break;
				}
				tmp = tmp->nextTermPtr;
			}
		}
		return;
	}

	void addLinkBasedPoly(LinkPolynomial& aPol, LinkPolynomial& bPol) 
	{
		LinkedPolynomialTerm* tmp = polynomialTermPtr, * a = aPol.polynomialTermPtr, * b = bPol.polynomialTermPtr;
		while (a||b)
		{
			if ( (a && !b ) || ( ( a && b ) && a->expo > b->expo ) )
			{
				if (!tmp)
				{
					tmp = polynomialTermPtr = new LinkedPolynomialTerm{ a->coef,a->expo };
				}
				else
				{
					tmp->nextTermPtr = new LinkedPolynomialTerm{ a->coef,a->expo };
					tmp = tmp->nextTermPtr;
				}
				a = a->nextTermPtr;
			}
			else if (  (a&&b) && a->expo==b->expo  )
			{
				if (!tmp)
				{
					if(a->coef+b->coef)
						tmp = polynomialTermPtr = new LinkedPolynomialTerm{ a->coef+b->coef,a->expo };
				}
				else if ( (a->coef+b->coef) !=0 ) {
					tmp->nextTermPtr = new LinkedPolynomialTerm{ a->coef + b->coef,a->expo };
					tmp = tmp->nextTermPtr;
				}
				a = a->nextTermPtr;
				b = b->nextTermPtr;
			}
			else
			{
				if (!tmp) {
					tmp = polynomialTermPtr = new LinkedPolynomialTerm{ b->coef, b->expo };
				}
				else {
					tmp->nextTermPtr = new LinkedPolynomialTerm{ b->coef, b->expo };
					tmp = tmp->nextTermPtr;
				}
				b = b->nextTermPtr;

			}
			
		}
		return;
	}

	void reverseLinkBasedPoly() 
	{
		if (!polynomialTermPtr || !polynomialTermPtr->nextTermPtr)
			return;

		LinkedPolynomialTerm *left = nullptr,*mid = polynomialTermPtr,*right = polynomialTermPtr->nextTermPtr;

		while (right) {
			mid->nextTermPtr = left;
			left = mid;
			mid = right;
			right = right->nextTermPtr;
		}
		mid->nextTermPtr = left;
		polynomialTermPtr = mid;

		return;
	}

	void printLinkBasedPoly()                                                            
	{
		LinkedPolynomialTerm* termPtr = polynomialTermPtr; 
		if (termPtr == nullptr)
			return;
		if (termPtr->expo == 0)
			cout << termPtr->coef;
		else
			cout << termPtr->coef << "X^" << termPtr->expo;
		termPtr = termPtr->nextTermPtr;
		while (termPtr != nullptr) {
			if (termPtr->coef == 0)
				return;
			if (termPtr->expo == 0)
				cout << " + " << termPtr->coef;
			else
				cout << " + " << termPtr->coef << "X^" << termPtr->expo;
			termPtr = termPtr->nextTermPtr;
		}
		return;
	}

	int compute(int x)
	{
		int all = 0;
		LinkedPolynomialTerm* t = polynomialTermPtr;
		while (t) {
			all += t->coef * pow(x, t->expo);
			t = t->nextTermPtr;
		}
		return all;
	}
};

int main()
{
	ArrayPolynomial a, b, d;                                                     
	LinkPolynomial aPol, bPol, dPol;                                             

	int coef, expo, x;

	// aPtr = bPtr = dPtr = nullptr; /////

	while (1) {
		a.clear(); b.clear(); d.clear();                                    
					  // aPtr->clear(aPtr); bPtr->clear(bPtr); dPtr->clear(dPtr); /////
		for (int i = 0; i < MAX_TERMS; i++) {
			cout << "\ninput a's coefficient and exponent: ";
			cin >> coef >> expo;
			if (expo == 0 && coef == 0)
				break;
			a.inputTerms(coef, expo);             
			aPol.inputLinkTerms(coef, expo); 
		}
		cout << "\n\na = ";
		a.printArrayBasedPoly();     
		cout << "\na = ";
		aPol.printLinkBasedPoly(); 
		cout << "\n";
		for (int i = 0; i < MAX_TERMS; i++) {
			cout << "\ninput b's coefficient and exponent: ";
			cin >> coef >> expo;
			if (expo == 0 && coef == 0)
				break;
			b.inputTerms(coef, expo);             
			bPol.inputLinkTerms(coef, expo); 
		}
		cout << "\n\nb = ";
		b.printArrayBasedPoly();    
		cout << "\nb = ";
		bPol.printLinkBasedPoly(); 
		cout << "\n";
		// d =a + b, where a, b, and d are polynomials
		d.addArrayBasedPoly(a, b); 
		cout << "\na + b = ";
		d.printArrayBasedPoly();     
		dPol.addLinkBasedPoly(aPol, bPol); 
		cout << "\na + b = ";
		dPol.printLinkBasedPoly();               
		cout << "\n";
		d.reverseArrayBasedPoly(); 
		cout << "\nreversed d = ";
		d.printArrayBasedPoly();     
		dPol.reverseLinkBasedPoly(); 
		cout << "\nreversed d = ";
		dPol.printLinkBasedPoly();   
		cout << "\n";
		cout << "\ninput X: ";
		cin >> x;
		cout << "\nd = ";
		cout << d.compute(x);      
		cout << "\nd = ";
		cout << dPol.compute(x); 
		cout << "\n";
		aPol.clear(); bPol.clear(); dPol.clear(); /////
	}
	return 0;
}