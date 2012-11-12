/* ===== semantic.c ===== */
#include <string>
#include <iostream>
#include <map>
#include <list>
#include <vector>


using namespace std;

#include <stdio.h>
#include <stdlib.h>
#include "ptype.hh"
#include "symtab.hh"

#include "myASTnode.hh"

#include "semantic.hh"

// feedback the main program with our error status
int TypeError = 0;

/// ---------- Error reporting routines --------------

void errornumparam(int l) {
  TypeError = 1;
  cout<<"L. "<<l<<": The number of parameters in the call do not match."<<endl;
}

void errorincompatibleparam(int l,int n) {
  TypeError = 1;
  cout<<"L. "<<l<<": Parameter "<<n<<" with incompatible types."<<endl;
}

void errorreferenceableparam(int l,int n) {
  TypeError = 1;
  cout<<"L. "<<l<<": Parameter "<<n<<" is expected to be referenceable but it is not."<<endl;
}

void errordeclaredident(int l, string s) {
  TypeError = 1;
  cout<<"L. "<<l<<": Identifier "<<s<<" already declared."<<endl;
}

void errornondeclaredident(int l, string s) {
  TypeError = 1;
  cout<<"L. "<<l<<": Identifier "<<s<<" is undeclared."<<endl;
}

void errornonreferenceableleft(int l, string s) {
  TypeError = 1;
  cout<<"L. "<<l<<": Left expression of assignment is not referenceable."<<endl;
}

void errorincompatibleassignment(int l) {
  TypeError = 1;
  cout<<"L. "<<l<<": Assignment with incompatible types."<<endl;
}

void errorbooleanrequired(int l,string s) {
  TypeError = 1;
  cout<<"L. "<<l<<": Instruction "<<s<<" requires a boolean condition."<<endl;
}

void errorisnotprocedure(int l) {
  TypeError = 1;
  cout<<"L. "<<l<<": Operator ( must be applied to a procedure in an instruction."<<endl;
}

void errorisnotfunction(int l) {
  TypeError = 1;
  cout<<"L. "<<l<<": Operator ( must be applied to a function in an expression."<<endl;
}

void errorincompatibleoperator(int l, string s) {
  TypeError = 1;
  cout<<"L. "<<l<<": Operator "<<s<<" with incompatible types."<<endl;
}

void errorincompatiblereturn(int l) {
  TypeError = 1;
  cout<<"L. "<<l<<": Return with incompatible type."<<endl;
}

void errorreadwriterequirebasic(int l, string s) {
  TypeError = 1;
  cout<<"L. "<<l<<": Basic type required in "<<s<<"."<<endl;
}

void errornonreferenceableexpression(int l, string s) {
  TypeError = 1;
  cout<<"L. "<<l<<": Referenceable expression required in "<<s<<"."<<endl;
}

void errornonfielddefined(int l, string s) {
  TypeError = 1;
  cout<<"L. "<<l<<": Field "<<s<<" is not defined in the struct."<<endl;
}

void errorfielddefined(int l, string s) {
  TypeError = 1;
  cout<<"L. "<<l<<": Field "<<s<<" already defined in the struct."<<endl;
}

/// ------------------------------------------------------------
/// Table to store information about program identifiers
symtab symboltable;

static void InsertintoST(int line,string kind,string id,ptype tp)
{
  infosym p;

  if (symboltable.find(id) && symboltable.jumped_scopes(id)==0) errordeclaredident(line,id);
  else {
    symboltable.createsymbol(id);
    symboltable[id].kind=kind;
    symboltable[id].tp=tp;
  }
}

/// ------------------------------------------------------------

bool isbasickind(string kind) {
  return kind=="int" || kind=="bool";
}



void check_params(AST *a,ptype tp,int line,int numparam)
{
  if (!a) return;
  // Contem num parametres a la crida
  int nParams = 0;
  for (AST *a1=a->down;a1!=0;a1=a1->right) {
    nParams++;
  }
  // Contem cuans n'hi hauria d'haver
  int nParams2 = 0;
  for (ttypenode *a1=tp->down;a1!=0;a1=a1->right) {
    nParams2++;
  }
  // Si no hi han els mateixos, error
  if (nParams != nParams2) {
    errornumparam(line);
  }else {
    int i = 0;
    tp = tp->down;
    for (AST *a1=a->down;a1!=0;a1=a1->right) {
      i++;
      TypeCheck(a1);
      if (tp->kind == "parref" && !a1->ref) {
        errorreferenceableparam(line,i);
      }else if(!equivalent_types(tp->down,a1->tp)) {
        errorincompatibleparam(line,i);
      }
      tp = tp->right;
    }
  }


}

void insert_params(AST *a) {
  if (!a) return;
  //TypeCheck(child(a,0));
  TypeCheck(child(a,1));
  InsertintoST(a->line, "idpar" + a->kind, child(a,0)->text, child(a,1)->tp );
  insert_params(a->right);

}
void insert_vars(AST *a)
{
  if (!a) return;
  TypeCheck(child(a,0));
  InsertintoST(a->line,"idvarlocal",a->text,child(a,0)->tp);
  insert_vars(a->right); 
}

void construct_struct(AST *a)
{
  AST *a1=child(a,0);
  a->tp=create_type("struct",0,0);

  while (a1!=0) {
    TypeCheck(child(a1,0));
    if (a->tp->struct_field.find(a1->text)==a->tp->struct_field.end()) {
      a->tp->struct_field[a1->text]=child(a1,0)->tp;
      a->tp->ids.push_back(a1->text);
     } else {
      errorfielddefined(a1->line,a1->text);
    }
    a1=a1->right;
  }
}

void construct_array(AST *a)
{
  TypeCheck(child(a,0));
  TypeCheck(child(a,1));
  AST *a0 = child(a,0);
  AST *a1 = child(a,1);
  a->tp = create_type("array",a1->tp,0);
  a->tp->numelemsarray = atoi(a0->text.c_str());
}

void create_params(AST *a) {
  TypeCheck(child(a, 1));
  if (a->right)
    {
      create_params(a->right);
      a->tp = create_type("par" + a->kind, child(a, 1)->tp, a->right->tp);
    }
  else
    {
      a->tp = create_type("par" + a->kind, child(a, 1)->tp, 0);
    }
}

void create_header(AST *a)
{
  AST *a1 = child(child(child(a,0),0),0);

  if (a1) create_params(a1);

  if(a->kind == "procedure") {
    if(a1) {
      a->tp = create_type(a->kind,a1->tp,0);
    }else {
      a->tp = create_type(a->kind,0,0);
    }
  }
  /*
  AST *ant = 0;
  int numParams = 1;
  for (AST *a1=a->down;a1!=0;a1=a1->right) {
    TypeCheck(child(a1,1));
    a1->tp =  create_type("par" + a1->kind, child(a1,1)->tp, 0);
    if (ant != 0) ant->right = a1;
    ant = a1;
  }
  */
}


void insert_header(AST *a)
{
  // Agafem la llista de parametres
  create_header(a);
  // Insertem el header a la symtab
  InsertintoST(a->line, "idproc", child(a,0)->text, a->tp);
}

void insert_headers(AST *a)
{
  while (a!=0) {
    insert_header(a);
    a=a->right;
  }
}


void TypeCheck(AST *a,string info)
{
  if (!a) {
    return;
  }

  //cout<<"Starting with node \""<<a->kind<<"\""<<endl;
  if (a->kind=="program") {
    a->sc=symboltable.push();
    insert_vars(child(child(a,0),0));
    
    insert_headers(child(child(a,1),0));
    TypeCheck(child(a,1));

    TypeCheck(child(a,2),"instruction");

    symboltable.pop();
  }
  else if (a->kind == "procedure") {
    a->sc=symboltable.push();
    // Insertem parametres d'entrada a la symtab.
    insert_params(child( child(child(a,0),0) ,0));
    //symboltable.write();
    insert_vars(child(child(a,1),0)); 
    insert_headers(child(child(a,2),0));
    TypeCheck(child(a,2));
    TypeCheck(child(a,3),"instruction");
    symboltable.pop();
  }
  else if (a->kind=="list") {
    // At this point only instruction, procedures or parameters lists are possible.
    for (AST *a1=a->down;a1!=0;a1=a1->right) {
      TypeCheck(a1,info);
    }
  } 
  else if (a->kind=="ident") {
    if (!symboltable.find(a->text)) {
      errornondeclaredident(a->line, a->text);
    } 
    else {
      a->tp=symboltable[a->text].tp;
      // Comprobem si l'expressio es referenciable
      if (a->tp->kind == "procedure")  a->ref=0;
      else a->ref = 1;
    }
  } 
  else if (a->kind=="struct") {
    construct_struct(a);
  }
  else if (a->kind == "array") {
    construct_array(a);
  }
  else if (a->kind==":=") {
    TypeCheck(child(a,0));
    TypeCheck(child(a,1));
    if (!child(a,0)->ref) {
      errornonreferenceableleft(a->line,child(a,0)->text);
    }
    else if (child(a,0)->tp->kind!="error" && child(a,1)->tp->kind!="error" &&
	     !equivalent_types(child(a,0)->tp,child(a,1)->tp)) {
      errorincompatibleassignment(a->line);
    } 
    else {
      a->tp=child(a,0)->tp;
    }
  } 
  else if (a->kind=="intconst") {
    a->tp=create_type("int",0,0);
  } 
  else if (a->kind=="+" || (a->kind=="-" && child(a,1)!=0) || a->kind=="*"
	   || a->kind=="/") {
    TypeCheck(child(a,0));
    TypeCheck(child(a,1));
    if ((child(a,0)->tp->kind!="error" && child(a,0)->tp->kind!="int") ||
	(child(a,1)->tp->kind!="error" && child(a,1)->tp->kind!="int")) {
      errorincompatibleoperator(a->line,a->kind);
    }
    a->tp=create_type("int",0,0);
  }
  else if (isbasickind(a->kind)) {
    a->tp=create_type(a->kind,0,0);
  }
  else if (a->kind=="writeln") {
    TypeCheck(child(a,0));
    if (child(a,0)->tp->kind!="error" && !isbasickind(child(a,0)->tp->kind)) {
      errorreadwriterequirebasic(a->line,a->kind);
    }
  }
  else if (a->kind==".") {
    TypeCheck(child(a,0));
    a->ref=child(a,0)->ref;
    if (child(a,0)->tp->kind!="error") {
      if (child(a,0)->tp->kind!="struct") {
	errorincompatibleoperator(a->line,"struct.");
      }
      else if (child(a,0)->tp->struct_field.find(child(a,1)->text)==
	       child(a,0)->tp->struct_field.end()) {
	errornonfielddefined(a->line,child(a,1)->text);
      } 
      else {
	a->tp=child(a,0)->tp->struct_field[child(a,1)->text];
      }
    }
  }
  else if (a->kind == "(") {
    TypeCheck(child(a,0));
    //cout << child(a,0)->tp->kind << " --- " << info << endl;
    if (child(a, 0)->tp->kind != "procedure" && info == "instruction") {
      errorisnotprocedure(a->line);       
    }else if (child(a, 0)->tp->kind != "function" && info != "instruction") {
      errorisnotfunction(a->line);
    }else {
      check_params (child(a,1), child(a,0)->tp, a->line, 0);
    }
  }
  else if (a->kind == "[") {
    TypeCheck(child(a,0));
    TypeCheck(child(a,1));
    a->ref=child(a,0)->ref;
    if (child(a,0)->tp->kind != "error") {
      if (child(a,0)->tp->kind != "array") {
        errorincompatibleoperator(a->line,"array[]");
      }
      else {
        a->tp=child(a,0)->tp->down;
        if (child(a,1)->tp->kind != "error" && child(a,1)->tp->kind != "int") {
          errorincompatibleoperator(a->line,"[]");
        } 
      }   
    }
  }
  else if (a->kind=="true" || a->kind=="false") {
    a->tp = create_type("bool",0,0);
  }
  else if (a->kind=="<" || a->kind==">") {
      TypeCheck(child(a,0));
      TypeCheck(child(a,1));
      if ((child(a,0)->tp->kind != "error" && child(a,0)->tp->kind != "int") ||
	 (child(a,1)->tp->kind != "error" && child(a,1)->tp->kind != "int")) {
	errorincompatibleoperator(a->line,a->kind);
      }
      a->tp = create_type("bool",0,0);
  }
  else if (a->kind=="or" || a->kind=="and") {
    TypeCheck(child(a,0));
    TypeCheck(child(a,1));
    if ((child(a,0)->tp->kind != "error" && child(a,0)->tp->kind != "bool") ||
	(child(a,1)->tp->kind != "error" && child(a,1)->tp->kind != "bool")) {
      	  errorincompatibleoperator(a->line,a->kind);
    }
    a->tp = create_type("bool",0,0);
  }
  else if (a->kind=="=") {
    TypeCheck(child(a,0));
    TypeCheck(child(a,1));
    if ((child(a,0)->tp->kind != "error" && child(a,1)->tp->kind != "error") && 
      (!equivalent_types(child(a,0)->tp,child(a,1)->tp) || 
      (!isbasickind(child(a,0)->tp->kind) || !isbasickind(child(a,1)->tp->kind))))
      errorincompatibleoperator(a->line,a->kind);
    a->tp = create_type("bool",0,0);
  }
  else if (a->kind == "not") {
      TypeCheck(child(a,0));
      if (child(a,0)->tp->kind != "error" && child(a,0)->tp->kind != "bool") 
	errorincompatibleoperator(a->line,a->kind);
      a->tp = create_type("bool",0,0);
  }
  else if (a->kind == "-") {
      TypeCheck(child(a,0));
      if (child(a,0)->tp->kind != "error" && child(a,0)->tp->kind != "int") 
	errorincompatibleoperator(a->line,a->kind);
      a->tp = create_type("int",0,0);
  }
  else if (a->kind == "if" || a->kind == "while") {
    TypeCheck(child(a,0));
    if (child(a,0)->tp->kind != "error" && child(a,0)->tp->kind != "bool" )
      errorbooleanrequired(a->line,a->kind);
    TypeCheck(child(a,1), "instruction");
    TypeCheck(child(a,2),"instruction");
  }
  else {
    cout<<"BIG PROBLEM en semantic.cc! No case defined for kind "<<a->kind<<endl;
  }

  //cout<<"Ending with node \""<<a->kind<<"\""<<endl;
}
