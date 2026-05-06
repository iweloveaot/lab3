#ifndef _LINEAR_FORM_UI_H_
#define _LINEAR_FORM_UI_H_

#include "linear_form.h"
#include "complex.h"
#include "for_all_ui.h"
#include <iostream>
#include <string>
#include <sstream>
#include <vector>

void showMainLinfMenu();
void linearFormMenu();
int selectLinearFormIndex(int count, const std::string& prompt = "Select linear form (0-");
void LinearFormUI();

template<typename T>
void printLinearForm(const LinearForm<T> &linf, const std::string &name = "");

template<typename T>
LinearForm<T> createLinearFormFromInput();

template<typename T>
void runUIForType();

#include "linear_form_ui.tpp"

#endif // _LINEAR_FORM_UI_H_