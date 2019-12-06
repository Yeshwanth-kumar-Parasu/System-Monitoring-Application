#ifndef PROCESSOR_H
#define PROCESSOR_H
class Processor 
{
 public:
  float Utilization();
 private:
  long Active_jiffies_start = 0;
  long Active_jiffies_end = 0;
  long Total_jiffies_start = 0;
  long Total_jiffies_end = 0;
};
#endif