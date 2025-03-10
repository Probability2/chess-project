#include "FileManager.hpp"

std::size_t FileManager::call_ = 0;

std::unique_ptr<Board> TxtManager::board_ = nullptr;

std::unique_ptr<GameState> FenManager::game_ = nullptr;

void TxtManager::SetValue(std::unique_ptr<Board> board) {
  if (!board) {
    return;
  }
  board_ = std::move(board);
}

void FenManager::SetValue(std::unique_ptr<GameState> state) {
  if (!state) {
    return;
  }
  game_ = std::move(state);
}

inline void SaveKings(std::unique_ptr<Board>& board, std::vector<std::vector<char>>& display) {
  display[ChessData::kMaxInd - (board->GetWhiteKing().GetPosition()[1] - '0')][board->GetWhiteKing().GetPosition()[0] - 'a'] = 'K';
  display[ChessData::kMaxInd - (board->GetBlackKing().GetPosition()[1] - '0')][board->GetBlackKing().GetPosition()[0] - 'a'] = 'k';
}

std::vector<std::vector<char>> GetPicture(std::unique_ptr<Board>& board) {
  std::vector<std::vector<char>> display(ChessData::kMaxInd, std::vector<char>(ChessData::kMaxInd, '.'));
  for (std::size_t i = 0; i < board->pieces_.size(); ++i) {
    for (const auto pos: board->pieces_[i]->GetPositions()) {
      display[ChessData::kMaxInd - (pos[1] - '0')][pos[0] - 'a'] = pchars_[i];
    }
  }
  SaveKings(board, display);

  return display;
}

void TxtManager::DownloadToFile(std::ofstream& file, const std::vector<std::vector<char>>& display) {
  for (std::size_t i = 0; i < ChessData::kMaxInd; ++i) {
    for (std::size_t j = 0; j < ChessData::kMaxInd; ++j) {
      file << display[i][j];
    }
    file << '\n';
  }
}

std::ofstream FileManager::CreateFile() {
  ++call_;
  fs::path directorypath = kDirectoryPrefix;
  if (!fs::exists(directorypath)) {
    fs::create_directory(directorypath); 
  }
  std::ofstream file(directorypath / CreateName(), std::ios::out);
  if (!file) {
    std::cerr << "Cannot create the file" << '\n';
    std::exit(EXIT_FAILURE);
  }

  return file;
}

void TxtManager::Save() {
  std::ofstream file = CreateFile();
  std::vector<std::vector<char>> vec;
  DownloadToFile(file, GetPicture(board_));
  file.close();
}

inline void FenManager::FillFenSkips(std::size_t& skip, std::ofstream& file) {
  if (skip > 0) {
    file << skip;
    skip = 0;
  }
}

void FenManager::ProcessFenRow(std::size_t& skip, std::vector<char>& vec, std::ofstream& file) {
  for (auto v: vec) {
    if (v != kEmptySquare) {
      FillFenSkips(skip, file);
      file << v;
      continue;
    }
    ++skip;
  }
}

void FenManager::ProcessFenBoard(std::ofstream& file) {
  std::vector<std::vector<char>> vec = GetPicture(game_->GetBoard());
  std::size_t skip = 0;
  for (auto& v: vec) {
    ProcessFenRow(skip, v, file);
    file << kFenDelimeter;
    FillFenSkips(skip, file);
  }
}

void FenManager::Save() {
  std::ofstream file = CreateFile();
  ProcessFenBoard(file);

  file.close();
}

std::string FileManager::CreateName() {
  return std::format("chess{}.txt", call_);
}

void FenManager::Get(const std::string& file_name) {
  std::ifstream file(file_name, std::ios::in);
  if (!file) {
    std::cerr << "File " << file_name << " is not found\n";
    return;
  }
  ReadFen(file);
  file.close();
}

void FenManager::ReadFen(std::ifstream& file) {
  char del;
  for (std::size_t i = 0; i < ChessData::kMaxCoord - 1; ++i) {
    ReadPlacement(file);
    file.read(&del, 1);
    if (file.gcount() != 1 || del != kFenDelimeter) {
      std::cerr << "Incorrect fen file\n";
      return;
    }
  }
  ReadPlacement(file);
  Parameters param;
  file.read(&del, 1);
  if (del != kSpaceDelimiter) {
    std::cerr << "Incorrect fen file\n";
    return;
  }
  SetTurn(file, param);
  file.read(&del, 1);
  if (del != kSpaceDelimiter) {
    std::cerr << "Incorrect fen file\n";
    return;
  }
  SetCastle(file, param);
  file.read(&del, 1);
  if (del != kSpaceDelimiter) {
    std::cerr << "Incorrect fen file\n";
    return;
  }
  SetEnPassant(file, param);
  file.read(&del, 1);
  if (del != kSpaceDelimiter) {
    std::cerr << "Incorrect fen file\n";
    return;
  }
  SetNoCaptures(file, param);
  file.read(&del, 1);
  if (del != kSpaceDelimiter) {
    std::cerr << "Incorrect fen file\n";
    return;
  }
  SetMove(file, param);
  game_->SetParameters(param);
}

void FenManager::SetTurn(std::ifstream& file, Parameters& param) {
  char turn;
  file.read(&turn, 1);
  param.SetWhiteMove(turn == 'w' ? true: false);
}

void FenManager::SetCastle(std::ifstream& file, Parameters& param) {
  char castle = '.';
  for (std::size_t i = 0; i < kCastleCount && castle != kSpaceDelimiter; ++i) {
    file.read(&castle, 1);
    GetCastle(castle, param);
  }
}

void FenManager::SetEnPassant(std::ifstream& file, Parameters& param) {
  char en;
  file.read(&en, 1);
}

void FenManager::SetNoCaptures(std::ifstream& file, Parameters& param) {
  std::size_t capture;
  file >> capture;
  param.SetNoCaptures(capture);
}

void FenManager::SetMove(std::ifstream& file, Parameters& param) {
  std::size_t move;
  file >> move;
  param.SetMove(move);
}

void FenManager::GetCastle(const char castle, Parameters& param) {
  switch(castle) {
    case 'K':
      param.SetWhiteShortCastle(true);
      return;
    case 'Q':
      param.SetWhiteLongCastle(true);
      return;
    case 'k':
      param.SetBlackShortCastle(true);
      return;
    case 'q':
      param.SetBlackShortCastle(true);
      return;
    default:
      std::cerr << "Incorrest castle input from FEN file\n";
  }
}

void FenManager::ReadPlacement(std::ifstream& file) {
  char buff[ChessData::kMaxCoord];
  file.read(buff, ChessData::kMaxCoord);
  for (std::size_t i = 0; i < file.gcount(); ++i) {
    std::string line;
    if (buff[i] >= '0' && buff[i] <= '9') {
      line.append(kEmptySquare, buff[i] - '0');
      continue;
    }
    line += buff[i];
    for (std::size_t i = 0; i < line.size(); ++i) {
      game_->board_->ReadPosition(line[i], std::format("{}{}", static_cast<char>(ChessData::kMinCoord + i), ChessData::kMaxCoord - i));
    }
    //std::cout << k << '\n';
  }
}

void TxtManager::Get(const std::string& file_name) {
  std::ifstream file(file_name, std::ios::in);
  if (!file) {
    std::cerr << "File " << file_name << " is not found\n";
    return;
  }
  ReadImage(file);
  file.close();
}

void TxtManager::ReadImage(std::ifstream& file) {
  std::string line;
  char k = '0' + ChessData::kMaxInd;
  while (std::getline(file, line) && k >= '1') {
    ProcessRow(line, k);
    --k;
  }
}

void TxtManager::ProcessRow(const std::string& line, const int k) {
  for (std::size_t i = 0; i < line.size(); ++i) {
    board_->ReadPosition(line[i], std::format("{}{}", static_cast<char>(ChessData::kMinCoord + i), k - '0'));
  }
  std::cout << k << '\n';
}