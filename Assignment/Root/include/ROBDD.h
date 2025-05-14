#pragma once

class ROBDD {
public: // types

	class Func;
	class Storage;
	
public: // constructor

	ROBDD();

	ROBDD(const ROBDD&) = delete;
	
	~ROBDD();

public: // operators

	ROBDD& operator=(const ROBDD&) = delete;

public: // methods
	
	const Func& genTrue();

	const Func& genFalse();

	const Func& genVar(unsigned uiVarNr);

	const Func& ite(const Func& crIf,const Func& crThen,const Func& crElse);

	bool isConstant(const Func& crFunc) const {
		return isTrue(crFunc) || isFalse(crFunc);
	}

	bool isTrue(const Func&) const;

	bool isFalse(const Func&) const;

	unsigned getVar(const Func&) const;

	const Func& getThen(const Func&) const;

	const Func& getElse(const Func&) const;
	
private: // members

	Storage* m_pStorage{nullptr};
};
