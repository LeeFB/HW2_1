//
//  FamilyTree.cpp
//  ancestor-tree-a
//
//  Created by Lee Fingerhut on 09/04/2020.
//  Copyright © 2020 Lee Fingerhut. All rights reserved.
//
#include <stdlib.h>
#include <string>
#include <iostream>
#include <stdexcept>
#include <exception>
#include "FamilyTree.hpp"
using namespace std;
using namespace family;
/*
 class myExc: public exception {
 public:
 string exc;
 myExc(string str) {
 exc = str;
 }
 virtual const char* what() const throw() {
 return exc.c_str();
 }
 };
 */
#define CHECK_THROW_STR(x, str) do {    \
if(x) {                             \
throw new std::string(str);           \
}                                   \
} while(0)

string Tree::getName() {
    return this->name;
}

void Tree::setName(string name) {
    this->name = name;
}

Tree* Tree::getFather() {
    return this->father;
}

void Tree::setFather(Tree* father) {
    this->father = father;
}

Tree* Tree::getMother() {
    return this->mother;
}
void Tree::setMother(Tree* mother) {
    this->mother = mother;
}

Tree* Tree::find_name_req(Tree* T, string name) {
    if(T->name == name) {
        return T;
    }
    Tree* t = NULL;
    if (T->father) {
        t = find_name_req(T->father, name);
        if (t) return t;
    }
    if (T->mother) {
        t = find_name_req(T->mother, name);
        if (t) return t;
    }
    return t ;
}

string Tree::find_name_level_req(Tree* T, string name, int current_level) {
    if(T == NULL)
    {
        return "";
    }
    if(T->father != NULL && T->father->getName() == name)
    {
        string s = "grandfather";
        for(int i=0; i<current_level; i++)
        {
            s = "great-"+s;
        }
        return s;
    }
    
    if(T->mother != NULL && T->mother->getName() == name)
    {
        string s = "grandmother";
        for(int i=0; i<current_level; i++)
        {
            s = "great-"+s;
        }
        return s;
    }
    string f = find_name_level_req(T->father, name, current_level+1);
    if(!f.empty())
    {
        return f;
    }
    string m = find_name_level_req(T->mother, name, current_level+1);
    if(!m.empty())
    {
        return m;
    }
    return "";
}

string Tree::member_rel(Tree* T, string name)
{
    if(name.empty() || T == NULL)
    {
        return "";
    }
    if(name == "grandfather")
    {
        if(T->father != NULL)
        {
            return T->father->getName();
        }
        return "";
    }
    if(name == "grandmother")
    {
        if(T->mother != NULL)
        {
            return T->mother->getName();
        }
        return "";
    }
    std::string s = name;
    std::string delimiter = "-";
    std::string token = s.substr(s.find(delimiter)+1);
    if(token == name)
    {
        return "";
    }
    cout << token << endl;
    string m = member_rel(T->mother, token);
    if(!m.empty())
    {
        return m;
    }
    string f = member_rel(T->father, token);
    if(!f.empty())
    {
        return f;
    }
    return "";
}

Tree& Tree::addFather(string name, string father) {
    Tree* Tname = find_name_req(this, name) ;
    CHECK_THROW_STR(!Tname, name+"does not exist");
    //if(!Tname)
    //{
    //    throw runtime_error (name+" does not exist");
    //}
    CHECK_THROW_STR(Tname->father, name+"alredy has a father");
    //if(Tname->father)
    //{
    //    throw runtime_error (name+" alredy has a father");
    //}
    Tname->father = new Tree(father) ;
    return *this;
}

Tree& Tree::addMother(string name, string mother) {
    Tree* Tname = find_name_req(this, name) ;
    CHECK_THROW_STR(!Tname, name+"does not exist");
    //if(!Tname)
    //{
    //    throw runtime_error (name+" does not exist");
    //}
    CHECK_THROW_STR(Tname->mother, name+"alredy has a mother");
    //if(Tname->mother)
    //{
    //    throw runtime_error ( name+" alredy has a mother");
    //}
    Tname->mother = new Tree(mother) ;
    return *this;
}

#define COUNT 10
void display_req(Tree* T, int space) {
    if (T->getName().empty()) {
        return ;
    }
    space += COUNT;
    if (T->getFather() != NULL) {
        display_req(T->getFather(), space);
    }
    cout << endl;
    for (int i = COUNT; i < space; i++) {
        cout<<" ";
    }
    cout << T->getName() << "\n";
    if (T->getMother() != NULL) {
        display_req(T->getMother(), space);
    }
}

void Tree::display() {
    display_req(this, 0);
}

string Tree::relation(string name) {
    CHECK_THROW_STR(name.empty(), "got empty string");
    //if(name.empty())
    //{
    //    throw runtime_error ("got empty string");
    //}
    if (this->name == name) {
        return "me" ;
    }
    if (this->father && this->father->name == name) {
        return "father" ;
    }
    if (this->mother && this->mother->name == name) {
        return "mother" ;
    }
    if(this->father) {
        string s1 = find_name_level_req(this->father, name, 0);
        if(!s1.empty()) {
            return s1;
        }
    }
    if(this->mother != NULL) {
        string s2 = find_name_level_req(this->mother, name, 0);
        if(!s2.empty()) {
            return s2;
        }
    }
    return "unrelated";
}

string Tree::find(string name) {
    //CHECK_THROW_STR(name.empty(), "got empty string");
    if(name.empty())
    {
        throw runtime_error ("got empty string");
    }
    int pos = name.find("-");
    cout << pos << endl;
    
    if (name == "me") {
        return this->name;
    }
    if (name == "father" && this->father) {
        return this->father->name;
    }
    if (name == "mother" && this->mother) {
        return this->mother->name;
    }
    if(this->father) {
        string s1 = member_rel(this->father, name);
        if(!s1.empty()) {
            return s1;
        }
    }
    if(this->mother != NULL) {
        string s2 = member_rel(this->mother, name);
        if(!s2.empty()) {
            return s2;
        }
    }
    //CHECK_THROW_STR(true, "The tree cannot handle the '"+name+"' relation");
    if(true)
    {
        throw runtime_error ("The tree cannot handle the '"+name+"' relation");
    }
}


void Tree::remove(string name) {
    if ( this->father && this->father->name == name) {
        delete this->father;
        this->father = NULL;
        return;
    }
    else if (this->mother && this->mother->name == name) {
        delete this->mother;
        this->mother = NULL;
        return;
    }
    else if(this->father) {
        this->father->remove(name);
    }
    else if (this->mother) {
        this->mother->remove(name);
    }
    else {
        throw runtime_error("can't delete");
    }
}