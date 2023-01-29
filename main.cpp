/*
 * Copyright (c) 2018 Torsten Muetze
 *
 * This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 2 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program.  If not, see <http://www.gnu.org/licenses/>.
 */

#include <cstdlib>
#include <iostream>
#include <vector>
#include <getopt.h>
// The following algorithms are implemented in Joerg Arndt's FXT library
#include "comb/partition.h"
#include "comb/mpartition.h"
#include "comb/setpart.h"

// display help
void help() {
  std::cout << "./part [options]  generate integer/set partitions" << std::endl;
  std::cout << "-h                display this help" << std::endl;
  std::cout << "-n{1,2,...}       size n of the integer/set" << std::endl;
  std::cout << "-m{1,2,...,n}     number of parts/subsets" << std::endl;
  std::cout << "-l{-1,0,1,2,...}  number of partitions to list; -1 for all" << std::endl;
  std::cout << "-a{0,1}           integer partitions (0*), set partitions (1)" << std::endl;  
  std::cout << "examples:  ./part -n8 -a0" << std::endl;
  std::cout << "           ./part -n8 -m3 -a0" << std::endl;
  std::cout << "           ./part -n5 -a1" << std::endl;
}

void opt_n_missing() {
  std::cerr << "options -n is mandatory" << std::endl;
}

int main(int argc, char *argv[]) {
  int n, m;
  bool n_set = false;  // flag whether option -n is present
  bool m_set = false;  // flag whether option -m is present
  bool set_partitions = false;  // compute integer partitions by default
  long long limit = -1;  // compute all partitions by default
  
  // process command line options
  int c;
  while ((c = getopt(argc, argv, ":hn:m:l:a:")) != -1) {
    switch (c) {
      case 'h':
        help();
        return 0;
      case 'n':
        n = atoi(optarg);
        if (n < 1) {
          std::cerr << "option -n must be followed by an integer from {1,2,...}" << std::endl;
          return 1;
        }
        n_set = true;
        break;
      case 'm':
        m = atoi(optarg);
        m_set = true;
        break;
      case 'l':
        limit = atoi(optarg);
        if (limit < -1) {
          std::cerr << "option -l must be followed by an integer from {-1,0,1,2,...}" << std::endl;
          return 1;
        }
        break;   
      case 'a':
        {
        int arg = atoi(optarg);
        if ((arg < 0) || (arg > 1)) {
          std::cerr << "option -a must be followed by 0 or 1" << std::endl;
          return 1;
        }
        set_partitions = (bool) arg;
        break;
        }        
      case ':':
        std::cerr << "option -" << (char) optopt << " requires an operand" << std::endl;
        return 1;
      case '?':
        std::cerr << "unrecognized option -" << (char) optopt << std::endl;
        return 1;
    }
  }
  if (!n_set) {
    opt_n_missing();
    help();
    return 1;
  }
  if (m_set && ((m < 1) || (m > n))) {
    std::cerr << "option -m must be followed by an integer from {1,2,...,n}" << std::endl;
    return 1;
  }

  // Compute the first limit integer/set partitions of an integer/set of size n.
  // A value limit < 0 means computing all of them.
  if (set_partitions) {
    // set partitions
    setpart x(n, true, 1);
    int count = 0;
    do {
        if ((limit >= 0) && (count >= limit)) { 
          std::cout << "output limit reached" << std::endl;
          break; 
        } 
        if (m_set && (x.ns_[n] != m)) { continue; }
        x.print();
        std::cout << std::endl;
        ++count;
    } while (x.next());    
  } else {
    // integer partitions 
    if (m_set) {
      // fixed number of parts 
      mpartition x(n, m);
      int count = 0;
      do {
        if ((limit >= 0) && (count >= limit)) { 
          std::cout << "output limit reached" << std::endl;
          break; 
        } 
        const ulong *dx = x.data();  // dx[0] + ... + dx[m-1] == n
        for (int k = m - 1; k >= 0; --k) {
          std::cout << dx[k] << " ";
        }
        std::cout << std::endl;
        ++count;
      } while (x.next());
    } else {
      // arbitrary number of parts
      partition x(n);
      const ulong *cx = x.data();  // cx[1]*1 + cx[2]*2 + ... + cx[n]*n == n
      x.first();
      int count = 0;
      do {
        if ((limit >= 0) && (count >= limit)) { 
          std::cout << "output limit reached" << std::endl;
          break; 
        } 
        // const ulong *cx = x.data();  // cx[1]*1 + cx[2]*2 + ... + cx[n]*n == n
        for (int k = 1; k <= n; k++) {
          if (cx[k] == 0) continue;
          for (int j = 1; j <= cx[k]; ++j) {
            std::cout << k << " ";
          }
          std::cout << "\n";
        }
        std::cout << std::endl;
        ++count;
      } while (x.next());
    }        
  }
  
  return 0;
}


