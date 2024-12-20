#pragma once


class FindMate {
public:
  virtual void Find() const = 0;

private:
  //Board board_;
};


class FindMateInOne: public FindMate {
public:
  void Find() const override {
  }

private:

};

class FindMateInTwo: public FindMate {
public:
  void Find() const override {

  }

private:

};

class FindMateInThree: public FindMate {
public:
  void Find() const override {

  }

private:

};

