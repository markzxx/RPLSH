#include <efanna.hpp>
#include <iostream>
#include <fstream>
#include <chrono>

using namespace efanna;
using namespace std;
void load_data(char* filename, float*& data, size_t& num,int& dim){// load data with sift10K pattern
  ifstream in(filename, ios::binary);
  if(!in.is_open()){cout<<"open file error"<<endl;exit(-1);}
  in.read((char*)&dim,4);
  cout<<"data dimension: "<<dim<<endl;
  in.seekg(0,ios::end);
  ios::pos_type ss = in.tellg();
  size_t fsize = (size_t)ss;
  num = fsize / (dim+1) / 4;
  data = new float[num*dim];

  in.seekg(0,ios::beg);
  for(size_t i = 0; i < num; i++){
    in.seekg(4,ios::cur);
    in.read((char*)(data+i*dim),dim*4);
  }
  in.close();
}
int main(int argc, char** argv){
  if(argc!=8){cout<< argv[0] << " index_file data_file query_file result_file table initsz querNN )" <<endl; exit(-1);}

  float* data_load = NULL;
  float* query_load = NULL;
  size_t points_num;
  int dim;
  load_data(argv[2], data_load, points_num,dim);
  size_t q_num;
  int qdim;
  load_data(argv[3], query_load, q_num,qdim);
  Matrix<float> dataset(points_num,dim,data_load);
  Matrix<float> query(q_num,qdim,query_load);

  FIndex<float> index(dataset, new L2DistanceAVX<float>(), efanna::HAMMINGIndexParams(32));
  index.loadIndex(argv[1]);


  int table = atoi(argv[5]);
  int poolsz = atoi(argv[6]);
  index.setSearchParams(table, poolsz);

  auto s = std::chrono::high_resolution_clock::now();

  index.knnSearch(atoi(argv[7])/*query nn*/,query,query_load);

  auto e = std::chrono::high_resolution_clock::now();
  std::chrono::duration<double> diff = e-s;
  std::cout << "query searching time: " << diff.count() << "\n";

  index.saveResults(argv[4]);

  return 0;
}
