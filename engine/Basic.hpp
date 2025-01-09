#pragma once

//#include "lib/ChessGame.hpp"

class Engine {
public:
  virtual void Process() = 0;

private:
  //GameState* state;

};

class BasicEngine: public Engine {
public:
  void Process() override;

private:
  
};