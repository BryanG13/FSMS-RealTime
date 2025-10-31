#include <string>
#include <fstream>
#include <iostream>
#include <stdlib.h> 
#include <random>
#include <time.h>
#include <omp.h>
#include <stdio.h>
#include <vector>
#include <sstream>
#ifdef _DEBUG
#define _CRTDBG_MAP_ALLOC
#define _CRTDBG_MAP_ALLOC_NEW
#include <crtdbg.h>
#include <assert.h>
#endif


using namespace std;
inline void read_walking_matrix(string path, double**& TT, int R, int S) {
	ifstream  data(path);
	string line;
	vector<vector<int>> bus_stops;
	vector<vector<int>> distance;
	vector<double> speed;

	size_t ind1, ind2;
	int l_it = 0;
	string sb1 = "]\"";
	string sb2 = "\"[";
	while (getline(data, line)) {
		vector<int> stops;
		vector<int> dist;
		stringstream lineStream(line);
		//cout << line << endl;
		string cell;
		vector<string> parsedRow;
		while (getline(lineStream, cell, ',')) {
			parsedRow.push_back(cell);
			cout << cell << " ----- ";
		}
		cout << endl;
		bool start = false;
		if (l_it != 0) {
			speed.push_back(stod(parsedRow[5]));
			int i = 6;
			//cout << "check\n";
			while(true) {
				ind1 = parsedRow[i].find(sb1);
				ind2 = parsedRow[i].find(sb2);
				if (ind1 != string::npos) {
					parsedRow[i].erase(ind1, sb1.length());
					//cout << parsedRow[i] << endl;
					//cout << "end\n";
					stops.push_back(stoi(parsedRow[i]));
					i++;
					break;
				}
				else if (ind2 != string::npos) {
					parsedRow[i].erase(ind2, sb2.length());
					//cout << parsedRow[i] << endl;
					stops.push_back(stoi(parsedRow[i]));
					start = true;
				}
				else {
					//cout << parsedRow[i] << endl;
					if (!start) {
						ind1 = parsedRow[i].find("[");
						ind2 = parsedRow[i].find("]");

						parsedRow[i].erase(ind1, 1);
						parsedRow[i].erase(ind2, 1);
						stops.push_back(stoi(parsedRow[i]));
						i++;
						break;

					}
					else stops.push_back(stoi(parsedRow[i]));
				}
				i++;
			}
			start = false;
			while (true) {
				ind1 = parsedRow[i].find(sb1);
				ind2 = parsedRow[i].find(sb2);
				if (ind1 != string::npos) {
					parsedRow[i].erase(ind1, sb1.length());
					dist.push_back(stoi(parsedRow[i]));
					break;
				}
				else if (ind2 != string::npos) {
					parsedRow[i].erase(ind2, sb2.length());
					dist.push_back(stoi(parsedRow[i]));
					start = true;
				}
				else {
					if (!start) {
						ind1 = parsedRow[i].find("[");
						ind2 = parsedRow[i].find("]");

						parsedRow[i].erase(ind1, 1);
						parsedRow[i].erase(ind2, 1);
						dist.push_back(stoi(parsedRow[i]));
						break;

					}
					else dist.push_back(stoi(parsedRow[i]));
				}
				i++;
			}
			
		}
		distance.push_back(dist);
		bus_stops.push_back(stops);
		l_it++;
	}
	int N_r = speed.size(), N_s = 0;

	for (int i = 0; i < N_r; i++) {
		N_s = distance[i].size();

		for (int j = 0; j < N_s; j++) {
			int s = bus_stops[i][j];
			int dist = distance[i][j];
			double time = dist / speed[i];
			TT[i][s] = time;
		}
	}
	//cout << " DATA\n";
	
	data.close();
	//exit(0);
}

inline void read_travel_matrix(string path, double**& TT, int L) {
	ifstream  data(path);
	string line;
	vector<vector<string>> parsedCsv;
	while (getline(data, line)) {
		stringstream lineStream(line);
		string cell;
		vector<string> parsedRow;
		while (getline(lineStream, cell, ',')) {
			parsedRow.push_back(cell);
		}
		//cout << parsedRow.size() << endl;
		parsedCsv.push_back(parsedRow);
	}

	//cout << " DATA\n";
	//cout << parsedCsv.size() << endl;
	//cout << "S " << L << endl;
	for (int i = 1; i <= L; i++) {
		//cout << parsedCsv[i].size() << endl;
		for (int j = 1; j <= L; j++) {
			TT[i - 1][j - 1] = stoi(parsedCsv[i][j])*0.58;
			//cout << TT[i - 1][j - 1] << "\t";
		}
		//cout << endl;
	}
	data.close();
	//exit(0);
}

inline int iMin(double bd[], int B) {
	double imin = INT32_MAX;
	int ind = -1;
	for (int i = 0; i < B; i++) {
		if (bd[i] < imin) {
			imin = bd[i];
			ind = i;
		}
	}
	return ind;
}
//second smallest of a set
inline double iMin2(double bd[], int B) {
	double first = INT_MAX, second = INT_MAX;
	for (int i = 0; i < B; i++) {
		/* If current element is smaller than first
		then update both first and second */
		if (bd[i] <= first) {
			second = first;
			first = bd[i];
		}

		/* If bd[i] is in between first and second
		then update second */
		else if (bd[i] < second && bd[i] != first) {
			second = bd[i];
		}
	}
	if (second == INT_MAX) return 0;
	else return second;
}

inline int partition(int index[], double dist[], int low, int high) {
	double pivot = dist[high]; // pivot  
	int i = (low - 1); // Index of smaller element 
	int t;
	double t0;

	for (int j = low; j <= high - 1; j++) {
		// If current element is smaller than the pivot  
		if (dist[j] < pivot) {
			i++; // increment index of smaller element  
			t = index[j];
			index[j] = index[i];
			index[i] = t;

			t0 = dist[j];
			dist[j] = dist[i];
			dist[i] = t0;

		}
	}
	t = index[high];
	index[high] = index[i + 1];
	index[i + 1] = t;

	t0 = dist[high];
	dist[high] = dist[i + 1];
	dist[i + 1] = t0;

	return (i + 1);
}

inline void quickSort(int index[], double dist[], int low, int high) {
	if (low < high) {
		/* pi is partitioning index, arr[p] is now
		at right place */
		int pi = partition(index, dist, low, high);

		// Separately sort elements before  
		// partition and after partition  
		quickSort(index, dist, low, pi - 1);
		quickSort(index, dist, pi + 1, high);
	}
}

inline int Qpartition(vector<double>& dist, int low, int high) {
	double pivot = dist[high]; // pivot  
	int i = (low - 1); // Index of smaller element  
	double t0;

	for (int j = low; j <= high - 1; j++) {
		// If current element is smaller than the pivot  
		if (dist[j] < pivot) {
			i++; // increment index of smaller element  
			t0 = dist[j];
			dist[j] = dist[i];
			dist[i] = t0;

		}
	}

	t0 = dist[high];
	dist[high] = dist[i + 1];
	dist[i + 1] = t0;

	return (i + 1);
}

inline void QSort(vector<double>& dist, int low, int high) {
	if (low < high) {
		/* pi is partitioning index, arr[p] is now
		at right place */
		int pi = Qpartition(dist, low, high);

		// Separately sort elements before  
		// partition and after partition  
		QSort(dist, low, pi - 1);
		QSort(dist, pi + 1, high);
	}
}

inline double findMedian(vector<double>& a) {
	// First we sort the array 
	int n = a.size();
	QSort(a, 0, n - 1);

	// check for even case 
	if (n % 2 != 0)
		return (double)a[n / 2];

	return (double)(a[(n - 1) / 2] + a[n / 2]) / 2.0;
}

inline int bestStop(vector<double>& timetable, vector<int>& route, double** traveltimes, double** traveltimep, int** closestPS,
	int p, int N, int dw, int* best_route, int M, int S, int bd, double* freqN, int xt, int best_stop,
	double arrprev, double arrnext, int d_tl, int d_te, int yk[][3], double* arrivals, int R1, int R2, int bus, int trip, double bd2, double fpm) {
	bool in = false, feas = true, inroute=false;
	int i, j, newj, cc, first, second, current, mand, indexr, indexr0=-1, best_index=-1;
	double sum, ener, ogtend;
	double ttemp, diffT = arrnext - arrprev, Arr, Arrb, D_temp = arrnext, D_sol, tArrb, startb, mF;
	vector<double> times, ttimetable;
	vector<int> troute;
	double UB;
	newj = closestPS[p][0];
	cc = 0;
	ener = INT64_MAX;
	sum = 0;
	first = 0, second = 0;
	indexr = -2;

	int nS = timetable.size();
	double* oldtt = new double[nS];// make copy of time table
	double* oldR = new double[nS];// make copy of route
	for (i = 0; i < nS; i++) {
		oldtt[i] = timetable[i];
		oldR[i] = route[i];
	}
	//cout << "p: " << p << " dat: " << arrnext / 60 << endl;
	while (traveltimep[p][closestPS[p][cc]] < dw) {
		//cout << cc << endl;
		sum = traveltimep[p][closestPS[p][cc]];
		mand = int((closestPS[p][cc] - N) / M);
		//cout << "mandatory : " << mand << endl;

		auto itr = find(best_route, best_route + S, closestPS[p][cc]);
		current = distance(best_route, itr);
		// find best place to insert cc
		in = false;
		int routeS = route.size();
		for (j = 0; j < routeS; j++) {
			if (route[j] == closestPS[p][cc]) {
				in = true;
				indexr = j;
				break;
			}
			else if (route[j] == mand || route[j] == mand + 1 || (route[j] >= N + M * mand && route[j] < N + M * (mand + 1))) {
				auto itr = find(best_route, best_route + S, route[j]);
				ttemp = distance(best_route, itr);
				//cout << ttemp << endl;
				if (ttemp > current) {
					second = route[j];
					indexr = j;
					break;
				}
				first = route[j];
				indexr0 = j;
			}
		}

		times.clear();
		times.push_back(arrnext);
		for (i = 0; i < R1; i++) {
			if (yk[i][0] == bus && yk[i][1] == trip)times.push_back(arrivals[i]);
		}
		if (times.size() != 1) {
			Arrb = findMedian(times);
			Arr = std::max(times.back() - d_tl, times[0]);
			UB = std::min(times[0] + d_te, times.back());
		}
		else {
			Arrb = arrnext;
			//cout << "first \n";
			Arr = arrnext - d_tl;
			UB = arrnext + d_te;
		}
		if (Arrb > UB)Arrb = UB;
		else if (Arrb < Arr)Arrb = Arr;
		startb = bd;
		tArrb = Arrb;
		//for (i = 0; i < times.size(); i++) {
			//cout << int(times[i] / 60 )<< "  ";
		//}
		//cout << endl;
		if (!in) {
			//cout << " stop: " << closestPS[p][cc] << " NOT in yet ---> UB: " <<UB/60 << " LB: "<< Arr/60 << endl;
			sum += traveltimes[first][closestPS[p][cc]] + traveltimes[closestPS[p][cc]][second] - traveltimes[first][second];
			for (i = 0; i < indexr0; i++) {
				startb += traveltimes[route[i]][route[i + 1]];
			}
			startb += (traveltimes[first][closestPS[p][cc]] + traveltimes[closestPS[p][cc]][second]);
			routeS = route.size() - 2;
			for (i = indexr; i < route.size() - 2; i++) {
				startb += traveltimes[route[i]][route[i + 1]];
			}

			ttimetable.clear();
			troute.clear();
			routeS = route.size() - 1;
			for (i = route.size() - 1; i > indexr; i--) {
				troute.insert(troute.begin(), route[i]);
				ttimetable.insert(ttimetable.begin(), tArrb);
				tArrb -= traveltimes[route[i]][route[i - 1]];
			}
			ttimetable.insert(ttimetable.begin(), tArrb);
			troute.insert(troute.begin(), route[indexr]);

			tArrb -= traveltimes[closestPS[p][cc]][second];
			ttimetable.insert(ttimetable.begin(), tArrb);
			troute.insert(troute.begin(), closestPS[p][cc]);

			tArrb -= traveltimes[closestPS[p][cc]][first];
			for (i = indexr0; i > 0; i--) {
				troute.insert(troute.begin(), route[i]);
				ttimetable.insert(ttimetable.begin(), tArrb);
				tArrb -= traveltimes[route[i]][route[i - 1]];
			}
			troute.insert(troute.begin(), route[0]);
			ttimetable.insert(ttimetable.begin(), tArrb);

		}
		else {//if stop is part of the route already, just check xt constraint
			//cout << " stop: " << closestPS[p][cc] << " IN already" << endl;
			routeS = route.size() - 2;
			for (i = 0; i < routeS; i++) {
				startb += traveltimes[route[i]][route[i + 1]];
			}
			ttimetable.clear();
			routeS = route.size() - 1;
			for (i = routeS; i >= 1; i--) {
				ttimetable.insert(ttimetable.begin(), tArrb);
				troute.insert(troute.begin(), route[i]);
				tArrb -= traveltimes[route[i]][route[i - 1]];
			}
			ttimetable.insert(ttimetable.begin(), tArrb);
			troute.insert(troute.begin(), route[0]);
		}
		if (startb > UB) {
			//cout << " route too long for p:" << p << endl;
			sum = INT32_MAX;
		}
		else {
			//cout << " poss dept at m0 " << ttimetable[0] / 60 << " poss arr: " << ttimetable.back()/60  << endl;
			//cout << "FreqN\n";
			//for (i = 0; i < N; i++) {
				//cout << int(freqN[i] / 60) << "  ";
			//}
			//cout << endl;
			//Check if infeasible;
			mF = -1;
			//cout << " start " << timetable[0]/60 << endl;
			int tttS = ttimetable.size();
			for (i = 0; i < tttS; i++) {
				if (troute[i]<N && ttimetable[i] - freqN[troute[i]]> mF && freqN[troute[i]] > 0)mF = ttimetable[i] - freqN[troute[i]];
			}
			//cout << "mF = " << mF / 60  << " xt: "  <<xt/60 << " d_tl: " << d_tl/60<<  endl;

			if (mF - xt > d_tl * fpm || ttimetable.back() - (mF - xt) < Arr) {
				//cout << " p: " << p << " ";
				//cout << " INfeasible for times: dif_mF=" << mF / 60 - xt / 60 << endl;
				//cout << " trip: " << trip << " bd: " << bd / 60 << " LB: " << Arr / 60 << " act arr: " << ttimetable.back() / 60 << endl;
				sum = INT32_MAX;
			}
			else if (mF - xt > 0) {
				//cout << " adjust\n";
				tttS = ttimetable.size();
				for (i = 0; i < tttS; i++) {
					ttimetable[i] -= (mF - xt);
				}
				sum += (mF - xt) * times.size();
			}

			if (ttimetable[0] < bd && trip>0) {
				//cout << "optimal not possible -> ASAP \n";
				//sum = INT32_MAX;
				//*
				ogtend = ttimetable.back();
				ttimetable.clear();
				startb = bd;
				ttimetable.push_back(startb);
				int trS = troute.size();
				for (i = 1; i < trS; i++) {
					startb += traveltimes[troute[i]][troute[i - 1]];
					ttimetable.push_back(startb);
				}
				if (abs(ogtend - ttimetable.back()) > d_tl * fpm || startb > UB || startb < Arr) {
					sum = INT32_MAX;
				}
				else sum += (abs(ogtend - ttimetable.back()) * times.size());
				//*/
			}
			//sum += (abs(Arrb - ttimetable.back()));
			D_temp = ttimetable.back();
			if (sum != INT32_MAX)sum += (abs(D_temp - arrnext));
		}

		if (indexr == -2) cout << "que?" << endl;
		if (ener > sum) {
			ener = sum;
			best_index = indexr;
			newj = closestPS[p][cc];
			inroute = in;
			D_sol = D_temp;
		}
		cc++;
	}
	//if no bus stop is avaibalbe
	if (ener == INT32_MAX) {
		//cout << "----------------------------------- no bus stop available \n";
		delete[] oldtt;
		delete[] oldR;
		return -1;
	}
	//add to route if not part of the route yet (if bestindex is not -1)
	if (!inroute) {
		//cout << " < -------------------   Stop " << newj << " not in route yet\n";
		route.insert(route.begin() + best_index, newj);
		//update timetable
		//cout <<" departure time " << D_sol/60 << endl;
		timetable.push_back(D_sol);
		//cout << route[best_index - 1] << "->" << newj << "\nloop: " <<endl;
		int tS = timetable.size() - 2;
		for (i = tS; i >= 0; i--) {
			timetable[i] = timetable[i + 1] - traveltimes[route[i + 1]][route[i]];
			//cout << route[i - 1] << "->" << route[i] << endl;
		}
	}
	else {
		timetable[timetable.size() - 1] = D_sol;
		//cout << route[best_index - 1] << "->" << newj << "\nloop: " <<endl;
		int tS = timetable.size() - 2;
		for (i = tS; i >= 0; i--) {
			timetable[i] = timetable[i + 1] - traveltimes[route[i + 1]][route[i]];
			//cout << route[i - 1] << "->" << route[i] << endl;
		}
	}
	/*
	cout << "Passenger: " << p << " Stop: " << newj << " Dept: " << int(D_sol / 60) <<" edt: "<<int(deptnext/60)<< endl;
	cout << "current timetable: \n";
	for (i = 0; i < timetable.size(); i++) {
		cout << int(timetable[i] / 60) << " ";
	}
	cout << endl;
	//*/
	//if this makes the next bus infeasible for freq constraint
	if (timetable[0] + xt < bd2) {
		timetable.clear();
		route.clear();
		for (i = 0; i < nS; i++) {
			timetable.push_back(oldtt[i]);
			route.push_back(oldR[i]);
		}
		delete[] oldtt;
		delete[] oldR;
		//cout << "----------------------------- BAD\nEarliest departure time to satisfy freqN: " << int(timetable[0] + xt) / 60 << endl;
		//cout << "Earliest departure time of of this bus is: " << int(bd2 / 60) << endl;
		return -1;
	}
	//cout << " +++++++++++++++++++++++++++++++++++++ ADDED on stop " << newj << " new arr: " << timetable.back()/60<< " \n";
	delete[] oldtt;
	delete[] oldR;
	return newj;
}

inline int bestStop2(vector<double>& timetable, vector<int>& route, double** traveltimes, double** traveltimep, int** closestPS,
	int p, int N, int dw, int* best_route, int M, int S, int bd, double* freqN, double* newfreqN, int xt, int best_stop,
	double deptprev, double deptnext, int d_tl, int d_te, int yk[][3], double* departures, int R1, int R2, int bus, int trip, double bd2, double fpm) {
	bool in = false, feas = true, found, inroute;
	int i, j, newj, cc, first, second, current, mand, indexr, indexr0, best_index, prev, i_m;
	double sum, ener, tsp, temp;
	double ttemp, threshold, diffT = deptnext - deptprev, Arr, Arrb, D_temp = deptnext, D_sol;
	vector<double> times;
	double t_prev = 0, UB;
	newj = closestPS[p][0];
	cc = 0;
	ener = INT64_MAX;
	sum = 0;
	first = 0, second = 0;
	indexr = -2;
	//cout << "p: " << p << endl;
	int nS = timetable.size();
	double* oldtt = new double[nS];// make copy of time table
	double* oldR = new double[nS];// make copy of route
	for (i = 0; i < nS; i++) {
		oldtt[i] = timetable[i];
		oldR[i] = route[i];
	}
	while (traveltimep[p][closestPS[p][cc]] < dw) {
		//cout << cc << endl;
		sum = traveltimep[p][closestPS[p][cc]];
		mand = int((closestPS[p][cc] - N) / M);
		//cout << "------- stop: " << closestPS[p][cc] << endl;
		//cout << "mandatory : " << mand << endl;

		auto itr = find(best_route, best_route + S, closestPS[p][cc]);
		current = distance(best_route, itr);
		// find best place to insert cc
		in = false;
		int rS = route.size();
		for (j = 0; j < rS; j++) {
			if (route[j] == closestPS[p][cc]) {
				in = true;
				indexr = j;
				break;
			}
			else if (route[j] == mand || route[j] == mand + 1 || (route[j] >= N + M * mand && route[j] < N + M * (mand + 1))) {
				auto itr = find(best_route, best_route + S, route[j]);
				ttemp = distance(best_route, itr);
				//cout << ttemp << endl;
				if (ttemp > current) {
					second = route[j];
					indexr = j;
					break;
				}
				first = route[j];
				indexr0 = j;
			}
		}
		//if cc is not part of the route add travel times
		auto itr2 = find(best_route, best_route + S, best_stop);
		prev = distance(best_route, itr2);//index of previous bus stop
		if (current < prev && diffT != 0) {
			sum = INT32_MAX;
		}
		else {
			if (!in) {
				t_prev = timetable[indexr0] + traveltimes[first][closestPS[p][cc]];
				if (t_prev > deptnext + d_te) sum = INT32_MAX;
				else {
					//cout << "earliest dept: " << (deptnext - d_tl)/60 << "earliest current arr: " << t_prev/60<<" stop: " << closestPS[p][cc]<< endl;
					temp = traveltimes[first][closestPS[p][cc]] + traveltimes[closestPS[p][cc]][second] - traveltimes[first][second];
					//cout << " extra travel time: " << temp / 60 << endl;
					feas = true;
					tsp = deptnext - d_tl - t_prev;//difference in time of arrival and desired time of departure at stop cc
					if (tsp < 0)tsp = 0;//if u arrive after edt -> no penalty
					i_m = -1;
					D_temp = -1;
					for (i = 0; i < N; i++) {
						if (i >= mand + 1) {
							if (int(newfreqN[i] + tsp + temp - freqN[i]) > xt && freqN[i] > 0) {
								feas = false;
								break;
							}
							//calculate max (only if feasible)
							if (newfreqN[i] + tsp + temp - freqN[i] > D_temp && freqN[i] > 0) {
								D_temp = newfreqN[i] + tsp + temp - freqN[i];
								i_m = i;
							}
						}
						else {
							if (int(newfreqN[i] + tsp - freqN[i]) > xt && freqN[i] > 0) {
								feas = false;
								break;
							}
							//calculate max (only if feasible)
							if (newfreqN[i] + tsp - freqN[i] > D_temp && freqN[i] > 0) {
								D_temp = newfreqN[i] + tsp - freqN[i];
								i_m = i;
							}
						}
					}
					if (feas) {
						sum += temp;
						if (diffT != 0) {
							//cout << "/////////////////////// D_temp=" << (D_temp) / 60 << endl;
							// time to go from prev to next stop on a full route 
							threshold = 0;
							found = false;
							for (i = 0; i < indexr - 1; i++) {
								if (found || route[i] == best_stop) {
									found = true;
									threshold += traveltimes[route[i]][route[i + 1]];
								}
							}
							threshold += traveltimes[route[i]][closestPS[p][cc]];
							sum += (abs(threshold - diffT));
						}
						if (diffT == 0) {
							//cout <<  "/////////////////////// D_temp=" << (D_temp) / 60 << endl;
							//D_temp = deptnext - (xt - D_temp);
							if (D_temp > 0) {
								double newF = deptnext;
								double coR = -1;
								newF -= traveltimes[first][closestPS[p][cc]];
								if (first < N) {
									if (newF - freqN[first] > coR)coR = newF - freqN[first];
								}
								for (i = indexr0; i > 0; i--) {
									newF -= traveltimes[route[i]][route[i - 1]];
									if (route[i - 1] < N) {
										if (newF - freqN[route[i - 1]] > coR)coR = newF - freqN[route[i - 1]];
									}
								}
								if (coR > xt) {
									newF = deptnext - (coR - xt);
									newF += traveltimes[second][closestPS[p][cc]];
									if (second < N) {
										if (newF - freqN[second] > xt)feas = false;
									}
									rS = route.size() - 1;
									for (i = indexr; i < rS; i++) {
										newF -= traveltimes[route[i]][route[i + 1]];
										if (route[i + 1] < N) {
											if (newF - freqN[route[i + 1]] > xt) {
												feas = false;
												break;
											}
										}
									}
									if (feas && coR - xt <= d_tl * fpm) D_temp = deptnext - (coR - xt);
									else sum = INT32_MAX;
								}
								else D_temp = deptnext;
								//cout << "/////////////////////// D_temp=" << (D_temp) / 60 << endl;
								//cout << "      CoR=" << (coR) / 60 << endl;
							}
							else {
								D_temp = deptnext;
								//cout << "NEG:://///////////////////// D_temp=" << (D_temp) / 60 << endl;
							}

						}
						else {
							if (D_temp > 0) {
								D_temp = xt - D_temp;
								D_temp = std::max(deptnext - d_tl + D_temp, t_prev);
							}
							else D_temp = deptnext;
						}

						if (D_temp > t_prev && diffT != 0) sum = INT32_MAX;
					}
					else sum = INT32_MAX;
				}

			}
			else {//if stop is part of the route already, just check xt constraint
				times.clear();
				times.push_back(deptnext);
				for (i = 0; i < R2; i++) {
					if (yk[i + R1][2] == closestPS[p][cc] && yk[i + R1][0] == bus && yk[i + R1][1] == trip)times.push_back(departures[i]);
				}
				if (times.size() != 1) {
					Arrb = findMedian(times);
					Arr = std::max(times.back() - d_tl, times[0]);
					UB = std::min(times[0] + d_te, times.back());
				}
				else {
					Arrb = deptnext;
					//cout << "first \n";
					Arr = deptnext - d_tl;
					UB = deptnext + d_te;
				}
				if (Arrb > UB)Arrb = UB;
				if (Arrb < Arr)Arrb = Arr;
				t_prev = timetable[indexr];
				if (t_prev > UB) sum = INT32_MAX;
				else {
					//cout << "earliest dept: " << (Arr) / 60 << " current arr: " << t_prev / 60 << " stop: " << closestPS[p][cc];
					tsp = Arr - t_prev;//difference in time of arrival and desired time of departure at stop cc
					if (tsp < 0)tsp = 0;//if u arrive after edt -> no penalty
					feas = true;
					i_m = -1;
					D_temp = -1;
					for (i = 0; i < N; i++) {
						if (int(newfreqN[i] + tsp - freqN[i]) > xt && freqN[i] > 0) {
							feas = false;
							break;
						}
						//calculate max (only if feasible)
						if (newfreqN[i] + tsp - freqN[i] > D_temp && freqN[i] > 0) {
							D_temp = newfreqN[i] + tsp - freqN[i];
							i_m = i;
						}
					}
					//cout << " biggest diff: " << D_temp/60 << endl;
					if (!feas) sum = INT32_MAX;
					else {
						//cout << "D_temp: " << D_temp/60 << endl;
						if (diffT == 0) {
							if (D_temp > 0) {
								//cout << "inroute -- >/////////////////////// D_temp=" << (D_temp) / 60 << endl;
								//D_temp = deptnext;

								double newF = deptnext;
								double coR = -1;
								if (closestPS[p][cc] < N) {
									if (newF - freqN[closestPS[p][cc]] > coR)coR = newF - freqN[closestPS[p][cc]];
								}
								for (i = indexr; i > 0; i--) {
									newF -= traveltimes[route[i]][route[i - 1]];
									if (route[i - 1] < N) {
										if (newF - freqN[route[i - 1]] > coR)coR = newF - freqN[route[i - 1]];
									}
								}
								if (coR > xt) {
									newF = deptnext - (coR - xt);
									rS = route.size() - 1;
									for (i = indexr; i < rS; i++) {
										newF -= traveltimes[route[i]][route[i + 1]];
										if (route[i + 1] < N) {
											if (newF - freqN[route[i + 1]] > xt) {
												feas = false;
												break;
											}
										}
									}
									if (feas && coR - xt <= d_tl * fpm) D_temp = deptnext - (coR - xt);
									else sum = INT32_MAX;
								}
								else D_temp = deptnext;
							}
							else D_temp = deptnext;
						}
						else {
							if (D_temp > 0) {
								D_temp = xt - D_temp;
								//cout << "D_temp: " << D_temp/60 << endl;
								D_temp = std::max(Arr + D_temp, t_prev);
							}
							else D_temp = Arrb;
						}
						if (D_temp > t_prev && diffT != 0) sum = INT32_MAX;
					}
				}

			}
		}
		if (sum < INT32_MAX) {
			double startb = D_temp;
			if (!in) {
				//cout << "NOT IN --> Check --> indexr0: " << indexr0 << " first: " << first <<endl;
				startb -= traveltimes[closestPS[p][cc]][first];
				for (i = indexr0; i > 0; i--) {
					startb -= traveltimes[route[i]][route[i - 1]];
				}
			}
			else {
				//cout << "IN --> Check --> indexr: " << indexr << endl;
				for (i = indexr; i > 0; i--) {
					startb -= traveltimes[route[i]][route[i - 1]];
				}
			}
			//cout << "Check 2\n";
			if (startb < bd && trip>0) {
				sum = INT32_MAX;
			}
		}

		if (indexr == -2) cout << "que?" << endl;
		if (ener > sum) {
			ener = sum;
			best_index = indexr;
			newj = closestPS[p][cc];
			inroute = in;
			D_sol = D_temp;
		}
		cc++;
	}
	//if no bus stop is avaibalbe
	if (ener == INT32_MAX) {
		//cout << " no bus stop available \n";
		delete[] oldtt;
		delete[] oldR;
		return -1;
	}
	//add to route if not part of the route yet (if bestindex is not -1)
	if (!inroute) {
		//cout << " < -------------------   Stop " << newj << " not in route yet\n";
		route.insert(route.begin() + best_index, newj);
		//update timetable
		//cout <<" departure time " << D_sol/60 << endl;
		timetable.insert(timetable.begin() + best_index, D_sol);
		//cout << route[best_index - 1] << "->" << newj << "\nloop: " <<endl;
		int tS = timetable.size();
		for (i = best_index + 1; i < tS; i++) {
			timetable[i] = timetable[i - 1] + traveltimes[route[i - 1]][route[i]];
			//cout << route[i - 1] << "->" << route[i] << endl;
		}
		if (diffT == 0) {
			for (i = best_index - 1; i >= 0; i--) {
				timetable[i] = timetable[i + 1] - traveltimes[route[i + 1]][route[i]];
				//cout << route[i - 1] << "->" << route[i] << endl;
			}
		}
	}
	else {
		//cout << " Stop " << route[best_index] << " already in route\t";
		//update timetable
		//cout << D_sol/60 << endl;
		//cout << " departure time " << D_sol / 60 << endl;
		timetable[best_index] = D_sol;
		int tS = timetable.size();
		for (i = best_index + 1; i < tS; i++) {
			timetable[i] = timetable[i - 1] + traveltimes[route[i - 1]][route[i]];
		}
		if (diffT == 0) {
			for (i = best_index - 1; i >= 0; i--) {
				timetable[i] = timetable[i + 1] - traveltimes[route[i + 1]][route[i]];
				//cout << route[i - 1] << "->" << route[i] << endl;
			}
		}
	}
	/*
	cout << "Passenger: " << p << " Stop: " << newj << " Dept: " << int(D_sol / 60) <<" edt: "<<int(deptnext/60)<< endl;
	cout << "current timetable: \n";
	for (i = 0; i < timetable.size(); i++) {
		cout << int(timetable[i] / 60) << " ";
	}
	cout << endl;
	//*/
	//if this makes the next bus infeasible for freq constraint
	if (timetable[0] + xt < bd2) {
		timetable.clear();
		route.clear();
		for (i = 0; i < nS; i++) {
			timetable.push_back(oldtt[i]);
			route.push_back(oldR[i]);
		}
		delete[] oldtt;
		delete[] oldR;
		//cout << "----------------------------- BAD\nEarliest departure time to satisfy freqN: " << int(timetable[0] + xt) / 60 << endl;
		//cout << "Earliest departure time of of this bus is: " << int(bd2 / 60) << endl;
		return -1;
	}

	delete[] oldtt;
	delete[] oldR;
	return newj;
}

inline double insertstop(vector<int>& b_route, vector<double>& b_timetable, int N, int M, int S, int OG_R, int OG_R1, int OG_R2, int OGxt, int d_dl, int d_de, int d_ae, int d_al, int d_t, double short_route, double* pickup, 
	double* OG_departures, double* OG_arrivals, int* best_route, double** traveltimes, double** traveltimep, int** closestPS, int dw, vector<vector<vector<double>>> FC, vector<vector<vector<int>>> b_xsol, 
	vector<vector<vector<double>>> b_Dsol, int ** b_ysol, int pt, int bus, int trip, double timestamp, bool driving, int prevstop, int nextstop, int max_wait, int& b_stop, float c1, float c2, float c3, float pm) {
	int i = 0, s = 0, nBus =0, j = 0, mand = 0;
	bool in = false;
	int in_i = -1, in_i0 = -1;
	int minF1 = INT16_MAX, minF2 = INT16_MAX, min_al = INT16_MAX, min_ae = INT16_MAX, min_dl = INT16_MAX, min_de = INT16_MAX,
		min_t1 = INT16_MAX, min_t2 = INT16_MAX, maxFW = 0, maxRW = 0, diffBD = 0, minxt1 = INT16_MAX, minxt2 = INT16_MAX, maxdb1 = INT16_MAX, maxdb2 = INT16_MAX, minwait = INT16_MAX;
	double diffa1 = 0, diffa2 = 0, diffd1 = 0, diffd2 = 0, diffF1 = 0, diffF2 = 0, difft1 = 0, difft2 = 0, diffxt1 = 0, diffxt2 = 0, diffwait = 0;
	double ttemp = 0, current =0;
	int first = 0, second = 0;
	double extratrav = 0;
	int dst = -1;
	double extracost = 0, cost3 = 0, mincost= INT32_MAX;
	vector<int> route;
	vector<double> timetable;
	bool FEAS = false;
	//int b_stop = -1;
	int p_onboard = 0, p1_onboard = 0 , p2_onboard = 0;

	//cout << endl << " BUS: " << bus << " TRIP: " << trip  << endl;
 	while (traveltimep[pt][closestPS[pt][i]] <= dw) { // given a bus trip and a passegner pt, go over feasible bus stops 
		nBus = b_xsol[bus][trip].size();
		route.clear();
		timetable.clear();
		extratrav = 0;
		s = closestPS[pt][i];
		//cout << "+++ Try stop " << s;
		if (s == N - 1) {
			i++;
			continue;
		}
		extracost = 0;
		//Determine if s in the route already
		mand = int((s - N) / M);
		auto itr = find(best_route, best_route + S, s);
		current = distance(best_route, itr);
		in = false;
		for (j = 0; j < nBus; j++) {
			if (b_xsol[bus][trip][j] == s) {
				in = true;
				in_i = j;
				in_i0 = in_i - 1;
				break;
			}
			else if (b_xsol[bus][trip][j] == mand || b_xsol[bus][trip][j] == mand + 1 || (b_xsol[bus][trip][j] >= N + M * mand && b_xsol[bus][trip][j] < N + M * (mand + 1))) {
				auto itr = find(best_route, best_route + S, b_xsol[bus][trip][j]);
				ttemp = distance(best_route, itr);
				//cout << ttemp << endl;
				if (ttemp > current) {
					second = b_xsol[bus][trip][j]; // stop in the exisitng route that should come after s, when s is inserted
					in_i = j;
					break;
				}
				first = b_xsol[bus][trip][j]; // stop in the exisitng route that should come before s, when s is inserted
				in_i0 = j; 
			}
		}
		
		//adjust timetable and route if needed  
		if (in) {
			// ++++++++ if part of route already
			//cout << " already in the route" << endl;
			for (j = 0; j < nBus; j++) {
				route.push_back(b_xsol[bus][trip][j]);
				timetable.push_back(b_Dsol[bus][trip][j]);
			}
			extratrav = 0;
		}
		else {
			//cout << " NOT in the route" << endl;
			if (second == nextstop) {
				i++;
				continue; // cannot insert a stop while the bus is driving towds it 
			}
			// ++++++++ if NOT part of route already
			extratrav = traveltimes[first][s] + traveltimes[s][second] - traveltimes[first][second]; // extra travel time
			//extracost += extratrav;

			for (j = 0; j <= in_i0; j++) {
				route.push_back(b_xsol[bus][trip][j]);
				timetable.push_back(b_Dsol[bus][trip][j]);
			}
			route.push_back(s);
			timetable.push_back(b_Dsol[bus][trip][in_i0] + traveltimes[first][s]);
			for (j = in_i; j < nBus; j++) {
				route.push_back(b_xsol[bus][trip][j]);
				timetable.push_back(b_Dsol[bus][trip][j] + extratrav);
			}
			nBus++;
		}
		
		int FWpt = 0, RWpt = 0;
		// ++++ new passenger pt requirements, to make solution feasible. This is also the max shift in the table allowed for the insertion of pt
		if (pt < OG_R1) { // if pt has arrival 
			FWpt = (OG_arrivals[pt] + d_al) - timetable[nBus - 1]; //if negative it means shift to the past, if positive to the future
			RWpt = (OG_arrivals[pt] - d_ae) - timetable[nBus - 1];
		}
		else { // if pt has departure
			FWpt = (OG_departures[pt - OG_R1] + d_dl) - timetable[in_i]; //if negative it means shift to the past, if positive to the future
			RWpt = (OG_departures[pt - OG_R1] - d_de) - timetable[in_i];
		}
		//cout << "RWpt: " << RWpt / 60 << " FWpt: " << FWpt / 60 << endl;
		/*
		cout << "current potential timetable\n";
		for (int tt = 0; tt < timetable.size(); tt++) {
			cout << timetable[tt] / 60 << " ";
		}
		cout << endl;
		//*/
		// max values: how much to modify timetable to make it feasible
		FEAS = true;
		minxt1 = INT16_MAX, minxt2 = INT16_MAX, min_al = INT16_MAX, min_ae = INT16_MAX, min_dl = INT16_MAX, min_de = INT16_MAX, min_t1 = INT16_MAX, min_t2 = INT16_MAX, minwait = INT16_MAX;

		double tdriving = 0;
		for (int ii = 0; ii < nBus; ii++) {
			if (timetable[ii] >= timestamp) {
				tdriving = timetable[ii];
				break;
			}
		}
		p_onboard = 0;
		p1_onboard = 0;
		p2_onboard = 0;
		for (int p = 0; p < OG_R; p++) {
			dst = -1;

			for (int ii = 0; ii < nBus; ii++) {
				if (b_ysol[p][0] == bus && b_ysol[p][1] == trip && b_ysol[p][2] == route[ii]) {
					dst = ii;
					break;
				}
			}
			if (dst != -1) {
				p_onboard++;
				if (p < OG_R1) {
					p1_onboard++;
					//cout << "pass " << p << endl;
					diffa1 = d_ae - (OG_arrivals[p] - timetable[nBus - 1]);
					diffa2 = d_al - (timetable[nBus - 1] - OG_arrivals[p]);
					if (min_ae > diffa1) {
						min_ae = diffa1;
					}
					if (min_al > diffa2) {
						min_al = diffa2;
					}
				}
				else {
					p2_onboard++;
					//cout << "passenger " << p +R1 << ": edt=" << int(departures[p] / 60) << " dept=" << int(timetable[dst] / 60) << " at stop: " <<route[dst] << endl;
					diffd1 = d_de - (OG_departures[p - OG_R1] - timetable[dst]);
					diffd2 = d_dl - (timetable[dst] - OG_departures[p - OG_R1]);
					if ( min_de > diffd1) {
						min_de = diffd1;
					}
					if ( min_dl > diffd2) {
						min_dl = diffd2;
					}
					if (timetable[dst] >= tdriving) {
						diffwait = d_dl - (timetable[dst] - OG_departures[p - OG_R1]);
						if (OG_departures[p - OG_R1] <= timetable[dst] && minwait > diffwait) {
							minwait = diffwait;
						}
					}
				}
				difft1 = d_t - (pickup[p] - timetable[dst]);
				difft2 = d_t - (timetable[dst] - pickup[p]);
				//if (difft2 < 0)cout << "p: "<< p << " tt: " << timetable[dst] / 60 << " pickup : " << pickup[p] / 60 << endl;
				if (min_t1 > difft1) {
					min_t1 = difft1;
				}
				if (min_t2 > difft2) {
					min_t2 = difft2;
				}
			}
		}
		
		minwait = min(minwait, min_al);
		int b_p = -1, t_p = -1;
		for (int ii = 0; ii < nBus; ii++) {
			if (route[ii] < N) {
				//if(pt==12 && bus ==1 && trip == 2 && s==13) cout << "Mandatory stop " << route[ii] << endl;
				minF1 = INT16_MAX, minF2 = INT16_MAX;
				//if (pt == 12 && bus == 1 && trip == 2 && s == 13)cout << ii << " tsize: " << nBus << " stop " << route[ii] << " max stops:"<< FC.size() <<endl;
				dst = FC[route[ii]].size();
				for (j = 0; j < dst; j++) {
					if (int(FC[route[ii]][j][1]) != bus || int(FC[route[ii]][j][2]) != trip) {
						//if (pt == 12 && bus == 1 && trip == 2 && s == 13)cout << "bus " << FC[route[ii]][j][1] << " trip " << FC[route[ii]][j][2] << " dept time other : " << int(FC[route[ii]][j][0] / 60) << " timetable diff : " << int(timetable[ii] / 60 - FC[route[ii]][j][0] / 60) << endl;
						diffF1 = (FC[route[ii]][j][0] - timetable[ii]);
						diffF2 = (timetable[ii] - FC[route[ii]][j][0]);
						if (FC[route[ii]][j][0] >= timetable[ii] && minF1 > diffF1) {
							minF1 = diffF1;
						}
						if (FC[route[ii]][j][0] <= timetable[ii] && minF2 > diffF2) {
							minF2 = diffF2;
							b_p = FC[route[ii]][j][1];
							t_p = FC[route[ii]][j][2];
						}
					}
				}
				if (minF2 == INT16_MAX) minF2 = 0;
				if (minF1 == INT16_MAX) minF1 = 0;
				//if (pt == 12 && bus == 1 && trip == 2 && s == 13)cout << " mindiff1 " << int(minF1/60) << " mindiff2 " << int(minF2/60) << endl;
				diffxt1 = OGxt - minF1;
				diffxt2 = OGxt - minF2;
				//if (pt == 12 && bus == 1 && trip == 2 && s == 13)cout << "--> mindiff1 " << int(diffxt1 / 60) << " mindiff2 " << int(diffxt2 / 60) << endl;
				if (minxt1 > diffxt1) {
					minxt1 = diffxt1;
				}
				if (minxt2 > diffxt2) {
					minxt2 = diffxt2;
				}
				if (mand < route[ii]) {
					diffwait = OGxt - minF2;
					if (minwait > diffwait) {
						minwait = diffwait;
					}
				}
			}
		}
		if (!in && b_p != -1) {
			//cout << " ****** BUS " << b_p << " TRIP " << t_p << endl;
			double minxt3 = INT16_MAX;
			int nBus_p = b_xsol[b_p][t_p].size();
			for (int ii = 0; ii < nBus_p; ii++) {
				if (b_xsol[b_p][t_p][ii] < N) {
					//if (pt == 12 && bus == 1 && trip == 2 && s == 13) cout << "Mandatory stop " << b_xsol[b_p][t_p][ii] << endl;
					minF1 = INT16_MAX;
					//if (pt == 12 && bus == 1 && trip == 2 && s == 13)cout << ii << " tsize: " << nBus << " stop " << b_xsol[b_p][t_p][ii] << " max stops:" << FC.size() << endl;
					dst = FC[b_xsol[b_p][t_p][ii]].size();
					for (j = 0; j < dst; j++) {
						if ((int(FC[b_xsol[b_p][t_p][ii]][j][1]) != b_p || int(FC[b_xsol[b_p][t_p][ii]][j][2]) != t_p) && (int(FC[b_xsol[b_p][t_p][ii]][j][1]) != bus || int(FC[b_xsol[b_p][t_p][ii]][j][2]) != trip)) {
							//if (pt == 12 && bus == 1 && trip == 2 && s == 13)cout << "bus " << FC[b_xsol[b_p][t_p][ii]][j][1] << " trip " << FC[b_xsol[b_p][t_p][ii]][j][2] << " dept time other : " << int(FC[b_xsol[b_p][t_p][ii]][j][0] / 60) << " timetable diff : " << (b_Dsol[b_p][t_p][ii] / 60 - FC[b_xsol[b_p][t_p][ii]][j][0] / 60) << endl;
							diffF1 = b_Dsol[b_p][t_p][ii] - FC[b_xsol[b_p][t_p][ii]][j][0];
							if (FC[b_xsol[b_p][t_p][ii]][j][0] <= b_Dsol[b_p][t_p][ii] && minF1 > diffF1) {
								minF1 = diffF1;
							}
						}
					}
					if (minF1 == INT16_MAX) minF1 = 0;
					//if (pt == 12 && bus == 1 && trip == 2 && s == 13)cout << " mindiff1 " << (minF1 / 60) << endl;
					diffxt1 = OGxt - minF1;
					//if (pt == 12 && bus == 1 && trip == 2 && s == 13)cout << "--> mindiff1 " << (diffxt1 / 60)  << endl;
					if (minxt3 > diffxt1) {
						minxt3 = diffxt1;
					}
				}
			}
			if (minxt3 < 0) FEAS = false;
		}

		if (FEAS) {
			if (trip != 0) {
				maxdb1 = timetable[0] - b_Dsol[bus][trip - 1][b_Dsol[bus][trip - 1].size() - 1] - short_route;
				//cout << "Maxdb (past): " << maxdb1 / 60 << endl;
				//maxRW = min(maxdb, maxRW);
			}
			if (trip != b_Dsol[bus].size() - 1) {
				maxdb2 = b_Dsol[bus][trip + 1][0] - short_route - timetable[nBus - 1];
				//cout << "Maxdb (future): " << maxdb2 / 60 << endl;
				//maxFW = min(maxdb, maxFW);
			}
			minwait = min(minwait, max_wait);
			//cout << " maxd1: " << min_de / 60 << " maxd2: " << min_dl / 60 << " maxa1: " << min_ae / 60 << " maxa2: " << min_al / 60 << " maxxt1: " << minxt1 / 60 << " maxxt2: " << minxt2 / 60 << " maxwait: " << minwait / 60 << " maxt1: " << min_t1 / 60 << " maxt2: " << min_t2 / 60 << " maxdb1: " << maxdb1 / 60 << " maxdb2: " << maxdb2 / 60 << endl;
			//cout << "MaxRW " << min(min(min(min(min_ae, min_de), min_t1), minxt1), maxdb2) / 60 << " maxFW: " << min(min(min(min(min_al, min_dl), min_t2), minxt2), maxdb2) / 60 << " maxwait: " << minwait / 60 << endl;
			if (!driving) {
				//cout << "not driving yet!\n";
				int UBs = 0, LBs = 0;
				if (min_al < 0 || min_dl < 0 || min_t2 < 0 || minxt2 < 0 || maxdb2 < 0) {// if the insertion of the stop made the solution infeasible (too much into the future)
					//cout <<" became infeasible bcs of stop insertion\n";
					maxFW = -min(min(min(min(min_al, min_dl), min_t2), minxt2), maxdb2); // correct with this value shift to the past 
					maxRW = min(min(min(min(min_ae, min_de), min_t1), minxt1), maxdb1);
					if (maxFW > maxRW) { // then infeasible
						FEAS = false;
					}
					else {
						// check bd feasibility
						if (trip != 0) {
							//adjust timetable between [maxFW,maxRW] in the past only
							int  shiftS = 0, shiftL = 0;
							if (FWpt < 0 && RWpt < 0) { //need to shift timetable to the past  at least shit maxFW, at most maxRW
								shiftS = -max(FWpt, RWpt); //most feasible shift 
								shiftL = -min(FWpt, RWpt); //least feasible shift 
								if (shiftS > maxRW || shiftL < maxFW) FEAS = false;
								else {
									UBs = min(shiftL, maxRW);//max to shift back 
									LBs = max(shiftS, maxFW);// min to shift back
									//cout << " ----> but can be fixed by shifting " << LBs / 60 << " to the past\n";
									for (j = 0; j < nBus; j++) {
										timetable[j] -= ((1 - pm) * LBs + UBs * pm);
									}
								}
							}
							else if (FWpt > 0 && RWpt > 0) { //cannot shift timetable to the future 
								FEAS = false;
							}
							else { //timetable already in the feasible region
								if (-RWpt < maxFW) FEAS = false;
								else {
									LBs = maxFW; //min to shift back 
									UBs = -RWpt; //max to shift back 
									//cout << " ----> but can be fixed by shifting " << LBs / 60 << " to the past\n";
									for (j = 0; j < nBus; j++) {
										timetable[j] -= ((1 - pm) * LBs + UBs * pm);
									}
								}
							}

						}
						else {
							//adjust timetable between [maxFW,maxRW] in the past only
							int  shiftS = 0, shiftL = 0;
							if (FWpt < 0 && RWpt < 0) { //need to shift timetable to the past  at least shit maxFW, at most maxRW
								shiftS = -max(FWpt, RWpt); //most feasible shift 
								shiftL = -min(FWpt, RWpt); //least feasible shift 
								if (shiftS > maxRW || shiftL < maxFW) FEAS = false;
								else {
									UBs = min(shiftL, maxRW);//max to shift back 
									LBs = max(shiftS, maxFW);// min to shift back
									//cout << " ----> but can be fixed by shifting " << LBs / 60 << " to the past\n";
									for (j = 0; j < nBus; j++) {
										timetable[j] -= ((1 - pm) * LBs + UBs * pm);;
									}
								}
							}
							else if (FWpt > 0 && RWpt > 0) { //cannot shift timetable to the future at most maxFW
								FEAS = false;
							}
							else { //timetable already in the feasible region
								if (-RWpt < maxFW) FEAS = false;
								else {
									LBs = maxFW; //min to shift back 
									UBs = -RWpt; //max to shift back 
									//cout << " ----> but can be fixed by shifting " << LBs / 60 << " to the past\n";
									for (j = 0; j < nBus; j++) {
										timetable[j] -= ((1 - pm) * LBs + UBs * pm);
									}
								}
							}
						}
					}
				}
				else {
					maxFW = -min(min(min(min(min_al, min_dl), min_t2), minxt2), maxdb2); // correct with this value shift to the past 
					maxRW = min(min(min(min(min_ae, min_de), min_t1), minxt1), maxdb1);
					int shiftS = 0, shiftL = 0;
					if (FWpt < 0 && RWpt < 0) { //need to shift timetable to the past at most maxRW
						shiftS = -max(FWpt, RWpt); //most feasible shift 
						shiftL = -min(FWpt, RWpt); //least feasible shift 
						if (shiftS > maxRW) FEAS = false;
						else {
							UBs = min(shiftL, maxRW); // max to shift back
							LBs = shiftS; // min to shift back
							//cout << "Needs shifting " << LBs / 60 << " to the past\n";
							for (j = 0; j < nBus; j++) {
								timetable[j] -= ((1 - pm) * LBs + UBs * pm);
							}
						}
					}
					else if (FWpt > 0 && RWpt > 0) { //need to shift timetable to the future at most maxFW
						shiftS = min(FWpt, RWpt); //most feasible shift 
						shiftL = max(FWpt, RWpt); //least feasible shift 
						if (shiftS > maxFW) FEAS = false;
						else if (shiftS > maxFW && shiftS <= minwait) {
							UBs = min(shiftL, minwait); // max to shift forwards
							LBs = shiftS; // min to shift forwards
							for (j = in_i; j < nBus; j++) {
								timetable[j] += ((1 - pm) * LBs + UBs * pm);
							}
						}
						else {
							UBs = min(shiftL, maxFW); // max to shift forwards
							LBs = shiftS; // min to shift forwards
							//cout << "Needs shifting " << LBs / 60 << " to the future\n";
							for (j = 0; j < nBus; j++) {
								timetable[j] += ((1 - pm) * LBs + UBs * pm);
							}
						}
					}
					else { //timetable already in the feasible region
						UBs = min(-RWpt, maxRW);// max to shift forwards
						LBs = min(FWpt, maxFW);// max to shift backwards
						//cout << "does not need  shifting \n";
						// Here we can shift timetable BUT DONT HAVE TO
						if (pm > 0.5) {
							for (j = 0; j < nBus; j++) {
								timetable[j] += (UBs * (pm - 0.5) * 2);
							}
						}
						else if (pm <= 0.5 && pm>0) {
							for (j = 0; j < nBus; j++) {
								timetable[j] -= (LBs * pm * 2);
							}
						}
					}

				}
			}
			else {
				if (timetable[in_i] < timestamp + traveltimep[pt][s] || min_al < 0 || min_dl < 0 || min_t2 < 0 || minxt2 < 0 || maxdb2 < 0) FEAS = false;
				else {
					int  shiftS = 0, shiftL = 0;
					int UBs = 0, LBs = 0;

					if (FWpt < 0 && RWpt < 0) { //need to shift timetable to the past  at least shit maxFW,but NOT POSSIBLE
						FEAS = false;
					}
					else if (FWpt > 0 && RWpt > 0) { //need to shift to the future, but NOT ALLOWED, unless we make buses wait ?? --> can wait at most "minwait"
						shiftS = min(FWpt, RWpt); //most feasible shift 
						shiftL = max(FWpt, RWpt); //least feasible shift 
						if (minwait < shiftS) FEAS = false;
						else {
							UBs = min(shiftL, minwait); // max to shift forwards
							LBs = shiftS; // min to shift forwards
							for (j = in_i; j < nBus; j++) {
								timetable[j] += ((1 - pm) * LBs + UBs * pm);
							}
						}
					}
					else { //timetable already in the feasible region
						UBs = min(-RWpt, maxRW);// max to shift forwards
						LBs = min(FWpt, maxFW);// max to shift backwards
						// Here we can shift timetable BUT DONT HAVE TO
						if (pm > 0.5) {
							for (j = 0; j < nBus; j++) {
								timetable[j] += (UBs * (pm - 0.5) * 2);
							}
						}
						else if (pm <= 0.5 && pm > 0) {
							for (j = 0; j < nBus; j++) {
								timetable[j] -= (LBs * pm * 2);
							}
						}
					}
				}
			}
		}
		if (FEAS) {
			//cout << "Passengers onboard " << p_onboard << endl;
			//Calculate added cost
			extracost = c2 * traveltimep[pt][s];
			if (pt < OG_R1)extracost += c3 * abs(OG_arrivals[pt] - timetable[nBus - 1]);
			else extracost += c3 * abs(OG_departures[pt - OG_R1] - timetable[in_i]);
			//cout << " cost: " << extracost << endl;
			extracost += c1 * (timetable[nBus - 1] - timetable[in_i]);
			//cout << " cost: " << extracost << endl;
			/*
			cout << "old tt: \n";
			for (int ii = 0; ii < b_Dsol[bus][trip].size(); ii++) {
				cout << b_Dsol[bus][trip][ii] / 60 << '\t';
			}
			cout << endl;
			cout << "new tt: \n";
			for (int ii = 0; ii < timetable.size(); ii++) {
				cout << timetable[ii] / 60 << '\t';
			}
			cout << endl;
			//*/
			for (int p = 0; p < OG_R; p++) {
				dst = -1;
				for (int ii = 0; ii < nBus; ii++) {
					if (b_ysol[p][0] == bus && b_ysol[p][1] == trip && b_ysol[p][2] == route[ii]) {
						dst = ii;
						break;
					}
				}
				if (dst != -1) {
					if (dst <= in_i) extracost += c1 * extratrav;
					if (p < OG_R1)extracost += c3 * (abs(OG_arrivals[p] - timetable[nBus - 1]) - abs(OG_arrivals[p] - b_Dsol[bus][trip][b_Dsol[bus][trip].size() - 1]));
					else {
						int dst0 = -1;
						for (int ii = 0; ii < b_xsol[bus][trip].size(); ii++) {
							if (b_ysol[p][0] == bus && b_ysol[p][1] == trip && b_ysol[p][2] == b_xsol[bus][trip][ii]) {
								dst0 = ii;
								break;
							}
						}
						extracost += c3 * (abs(OG_departures[p - OG_R1] - timetable[dst]) - abs(OG_departures[p - OG_R1] - b_Dsol[bus][trip][dst0]));
					}
				}
			}
			//cout << "******* Is FEASIBLE with stop " << s << " with cost = " << extracost;


			if (mincost > extracost) {
				//cout << " AND is also better ";
				mincost = extracost;
				b_route.clear();
				b_timetable.clear();
				for (j = 0; j < nBus; j++) {
					b_route.push_back(route[j]);
					b_timetable.push_back(timetable[j]);
				}
				b_stop = s;
			}
			//cout << endl;
		}
		//else cout << " NOT FEASIBLE with stop " << s << " and cost " <<  mincost << endl;
		i++; 
	}

	if (mincost == INT32_MAX) {
		//cout << "--> NOT FEASIBLE "  << endl;
		return -1;
	}
	else {		
		//cout << "******* Is BEST FEASIBLE with stop " << s << " with cost = " << mincost << endl;
		return mincost;
	}
}

inline double insertfixedstop(vector<int>& b_route, vector<double>& b_timetable, int N, int M, int S, int OG_R, int OG_R1, int OG_R2, int OGxt, int d_dl, int d_de, int d_ae, int d_al, int d_t, double short_route, double* pickup,
	double* OG_departures, double* OG_arrivals, int* best_route, double** traveltimes, double** traveltimep, int dw, vector<vector<vector<double>>> FC, vector<vector<vector<int>>> b_xsol,
	vector<vector<vector<double>>> b_Dsol, int** b_ysol, int pt, int bus, int trip,  int& b_stop, float c1, float c2, float c3, int pickupstop, float pm, int max_wait, double timestamp) {
	int i = 0, s = 0, nBus = 0, j = 0, mand = 0;
	bool in = false;
	int in_i = -1, in_i0 = -1;
	int minF1 = INT16_MAX, minF2 = INT16_MAX, min_al = INT16_MAX, min_ae = INT16_MAX, min_dl = INT16_MAX, min_de = INT16_MAX,
		min_t1 = INT16_MAX, min_t2 = INT16_MAX, maxFW = 0, maxRW = 0, diffBD = 0, minxt1 = INT16_MAX, minxt2 = INT16_MAX, maxdb1 = INT16_MAX, maxdb2 = INT16_MAX, minwait = INT16_MAX;
	double diffa1 = 0, diffa2 = 0, diffd1 = 0, diffd2 = 0, diffF1 = 0, diffF2 = 0, difft1 = 0, difft2 = 0, diffxt1 = 0, diffxt2 = 0, diffwait = 0;
	double ttemp = 0, current = 0;
	int first = 0, second = 0;
	double extratrav = 0;
	int dst = -1;
	double extracost = 0, cost3 = 0, mincost = INT32_MAX;
	vector<int> route;
	vector<double> timetable;
	bool FEAS = false;
	//int b_stop = -1;
	int p_onboard = 0, p1_onboard = 0, p2_onboard = 0;

	//cout << endl << " BUS: " << bus << " TRIP: " << trip <<"(fixed passenger) "<< endl;
	nBus = b_xsol[bus][trip].size();
	route.clear();
	timetable.clear();
	extratrav = 0;
	s = pickupstop;

	extracost = 0;
	//Determine if s in the route already
	mand = int((s - N) / M);
	auto itr = find(best_route, best_route + S, s);
	current = distance(best_route, itr);
	in = false;
	for (j = 0; j < nBus; j++) {
		if (b_xsol[bus][trip][j] == s) {
			in = true;
			in_i = j;
			in_i0 = in_i - 1;
			break;
		}
		else if (b_xsol[bus][trip][j] == mand || b_xsol[bus][trip][j] == mand + 1 || (b_xsol[bus][trip][j] >= N + M * mand && b_xsol[bus][trip][j] < N + M * (mand + 1))) {
			auto itr = find(best_route, best_route + S, b_xsol[bus][trip][j]);
			ttemp = distance(best_route, itr);
			//cout << ttemp << endl;
			if (ttemp > current) {
				second = b_xsol[bus][trip][j]; // stop in the exisitng route that should come after s, when s is inserted
				in_i = j;
				break;
			}
			first = b_xsol[bus][trip][j]; // stop in the exisitng route that should come before s, when s is inserted
			in_i0 = j;
		}
	}

	//adjust timetable and route if needed  
	if (in) {
		// ++++++++ if part of route already
		//cout << " already in the route" << endl;
		for (j = 0; j < nBus; j++) {
			route.push_back(b_xsol[bus][trip][j]);
			timetable.push_back(b_Dsol[bus][trip][j]);
		}
		extratrav = 0;
	}
	else {
		//cout << " NOT in the route" << endl;
		// ++++++++ if NOT part of route already
		//cout << " first " << first << " second " << second << endl;
		extratrav = traveltimes[first][s] + traveltimes[s][second] - traveltimes[first][second]; // extra travel time
		//extracost += extratrav;

		for (j = 0; j <= in_i0; j++) {
			route.push_back(b_xsol[bus][trip][j]);
			timetable.push_back(b_Dsol[bus][trip][j]);
		}
		route.push_back(s);
		timetable.push_back(b_Dsol[bus][trip][in_i0] + traveltimes[first][s]);
		for (j = in_i; j < nBus; j++) {
			route.push_back(b_xsol[bus][trip][j]);
			timetable.push_back(b_Dsol[bus][trip][j] + extratrav);
		}
		nBus++;
	}

	int FWpt = 0, RWpt = 0;
	// ++++ new passenger pt requirements, to make solution feasible. This is also the max shift in the table allowed for the insertion of pt
	if (pt < OG_R1) { // if pt has arrival 
		FWpt = (OG_arrivals[pt] + d_al) - timetable[nBus - 1]; //if negative it means shift to the past, if positive to the future
		RWpt = (OG_arrivals[pt] - d_ae) - timetable[nBus - 1];
	}
	else { // if pt has departure
		FWpt = (OG_departures[pt - OG_R1] + d_dl) - timetable[in_i]; //if negative it means shift to the past, if positive to the future
		RWpt = (OG_departures[pt - OG_R1] - d_de) - timetable[in_i];
	}
	//cout << "RWpt: " << RWpt / 60 << " FWpt: " << FWpt / 60 << endl;
	/*
	cout << "current potential timetable\n";
	for (int tt = 0; tt < timetable.size(); tt++) {
		cout << timetable[tt] / 60 << " ";
	}
	cout << endl;
	//*/
	// max values: how much to modify timetable to make it feasible
	FEAS = true;
	minxt1 = INT16_MAX, minxt2 = INT16_MAX, min_al = INT16_MAX, min_ae = INT16_MAX, min_dl = INT16_MAX, min_de = INT16_MAX, min_t1 = INT16_MAX, min_t2 = INT16_MAX;

	double tdriving = 0;
	for (int ii = 0; ii < nBus; ii++) {
		if (timetable[ii] >= timestamp) {
			tdriving = timetable[ii];
			break;
		}
	}
	p_onboard = 0;
	p1_onboard = 0;
	p2_onboard = 0;
	for (int p = 0; p < OG_R; p++) {
		dst = -1;

		for (int ii = 0; ii < nBus; ii++) {
			if (b_ysol[p][0] == bus && b_ysol[p][1] == trip && b_ysol[p][2] == route[ii]) {
				dst = ii;
				break;
			}
		}
		if (dst != -1) {
			p_onboard++;
			if (p < OG_R1) {
				p1_onboard++;
				//cout << "pass " << p << endl;
				diffa1 = d_ae - (OG_arrivals[p] - timetable[nBus - 1]);
				diffa2 = d_al - (timetable[nBus - 1] - OG_arrivals[p]);
				if (min_ae > diffa1) {
					min_ae = diffa1;
				}
				if (min_al > diffa2) {
					min_al = diffa2;
				}
			}
			else {
				p2_onboard++;
				//cout << "passenger " << p +R1 << ": edt=" << int(departures[p] / 60) << " dept=" << int(timetable[dst] / 60) << " at stop: " <<route[dst] << endl;
				diffd1 = d_de - (OG_departures[p - OG_R1] - timetable[dst]);
				diffd2 = d_dl - (timetable[dst] - OG_departures[p - OG_R1]);
				if (min_de > diffd1) {
					min_de = diffd1;
				}
				if (min_dl > diffd2) {
					min_dl = diffd2;
				}
				if (timetable[dst] >= tdriving) {
					diffwait = d_dl - (timetable[dst] - OG_departures[p - OG_R1]);
					if (OG_departures[p - OG_R1] <= timetable[dst] && minwait > diffwait) {
						minwait = diffwait;
					}
				}
			}
			difft1 = d_t - (pickup[p] - timetable[dst]);
			difft2 = d_t - (timetable[dst] - pickup[p]);
			//if (difft2 < 0)cout << "p: "<< p << " tt: " << timetable[dst] / 60 << " pickup : " << pickup[p] / 60 << endl;
			if (min_t1 > difft1) {
				min_t1 = difft1;
			}
			if (min_t2 > difft2) {
				min_t2 = difft2;
			}

		}
	}
	difft1 = d_t - (pickup[pt] - timetable[in_i]);
	difft2 = d_t - (timetable[in_i] - pickup[pt]);
	if (min_t1 > difft1) {
		min_t1 = difft1;
	}
	if (min_t2 > difft2) {
		min_t2 = difft2;
	}
	minwait = min(minwait, min_al);

	int b_p = -1, t_p = -1;
	for (int ii = 0; ii < nBus; ii++) {
		if (route[ii] < N) {
			//if(pt==12 && bus ==1 && trip == 2 && s==13) cout << "Mandatory stop " << route[ii] << endl;
			minF1 = INT16_MAX, minF2 = INT16_MAX;
			//if (pt == 12 && bus == 1 && trip == 2 && s == 13)cout << ii << " tsize: " << nBus << " stop " << route[ii] << " max stops:"<< FC.size() <<endl;
			dst = FC[route[ii]].size();
			for (j = 0; j < dst; j++) {
				if (int(FC[route[ii]][j][1]) != bus || int(FC[route[ii]][j][2]) != trip) {
					//if (pt == 12 && bus == 1 && trip == 2 && s == 13)cout << "bus " << FC[route[ii]][j][1] << " trip " << FC[route[ii]][j][2] << " dept time other : " << int(FC[route[ii]][j][0] / 60) << " timetable diff : " << int(timetable[ii] / 60 - FC[route[ii]][j][0] / 60) << endl;
					diffF1 = (FC[route[ii]][j][0] - timetable[ii]);
					diffF2 = (timetable[ii] - FC[route[ii]][j][0]);
					if (FC[route[ii]][j][0] >= timetable[ii] && minF1 > diffF1) {
						minF1 = diffF1;
					}
					if (FC[route[ii]][j][0] <= timetable[ii] && minF2 > diffF2) {
						minF2 = diffF2;
						b_p = FC[route[ii]][j][1];
						t_p = FC[route[ii]][j][2];
					}
				}
			}
			if (minF2 == INT16_MAX) minF2 = 0;
			if (minF1 == INT16_MAX) minF1 = 0;
			//if (pt == 12 && bus == 1 && trip == 2 && s == 13)cout << " mindiff1 " << int(minF1/60) << " mindiff2 " << int(minF2/60) << endl;
			diffxt1 = OGxt - minF1;
			diffxt2 = OGxt - minF2;
			//if (pt == 12 && bus == 1 && trip == 2 && s == 13)cout << "--> mindiff1 " << int(diffxt1 / 60) << " mindiff2 " << int(diffxt2 / 60) << endl;
			if (minxt1 > diffxt1) {
				minxt1 = diffxt1;
			}
			if (minxt2 > diffxt2) {
				minxt2 = diffxt2;
			}
			if (mand < route[ii]) {
				diffwait = OGxt - minF2;
				if (minwait > diffwait) {
					minwait = diffwait;
				}
			}
		}
	}
	if (!in && b_p != -1) {
		//cout << " ****** BUS " << b_p << " TRIP " << t_p << endl;
		double minxt3 = INT16_MAX;
		int nBus_p = b_xsol[b_p][t_p].size();
		for (int ii = 0; ii < nBus_p; ii++) {
			if (b_xsol[b_p][t_p][ii] < N) {
				//if (pt == 12 && bus == 1 && trip == 2 && s == 13) cout << "Mandatory stop " << b_xsol[b_p][t_p][ii] << endl;
				minF1 = INT16_MAX;
				//if (pt == 12 && bus == 1 && trip == 2 && s == 13)cout << ii << " tsize: " << nBus << " stop " << b_xsol[b_p][t_p][ii] << " max stops:" << FC.size() << endl;
				dst = FC[b_xsol[b_p][t_p][ii]].size();
				for (j = 0; j < dst; j++) {
					if ((int(FC[b_xsol[b_p][t_p][ii]][j][1]) != b_p || int(FC[b_xsol[b_p][t_p][ii]][j][2]) != t_p) && (int(FC[b_xsol[b_p][t_p][ii]][j][1]) != bus || int(FC[b_xsol[b_p][t_p][ii]][j][2]) != trip)) {
						//if (pt == 12 && bus == 1 && trip == 2 && s == 13)cout << "bus " << FC[b_xsol[b_p][t_p][ii]][j][1] << " trip " << FC[b_xsol[b_p][t_p][ii]][j][2] << " dept time other : " << int(FC[b_xsol[b_p][t_p][ii]][j][0] / 60) << " timetable diff : " << (b_Dsol[b_p][t_p][ii] / 60 - FC[b_xsol[b_p][t_p][ii]][j][0] / 60) << endl;
						diffF1 = b_Dsol[b_p][t_p][ii] - FC[b_xsol[b_p][t_p][ii]][j][0];
						if (FC[b_xsol[b_p][t_p][ii]][j][0] <= b_Dsol[b_p][t_p][ii] && minF1 > diffF1) {
							minF1 = diffF1;
						}
					}
				}
				if (minF1 == INT16_MAX) minF1 = 0;
				//if (pt == 12 && bus == 1 && trip == 2 && s == 13)cout << " mindiff1 " << (minF1 / 60) << endl;
				diffxt1 = OGxt - minF1;
				//if (pt == 12 && bus == 1 && trip == 2 && s == 13)cout << "--> mindiff1 " << (diffxt1 / 60)  << endl;
				if (minxt3 > diffxt1) {
					minxt3 = diffxt1;
				}
			}
		}
		if (minxt3 < 0) FEAS = false;
	}

	if (FEAS) {
		if (trip != 0) {
			maxdb1 = timetable[0] - b_Dsol[bus][trip - 1][b_Dsol[bus][trip - 1].size() - 1] - short_route;
			//cout << "Maxdb (past): " << maxdb1 / 60 << endl;
			//maxRW = min(maxdb, maxRW);
		}
		if (trip != b_Dsol[bus].size() - 1) {
			maxdb2 = b_Dsol[bus][trip + 1][0] - short_route - timetable[nBus - 1];
			//cout << "Maxdb (future): " << maxdb2 / 60 << endl;
			//maxFW = min(maxdb, maxFW);
		}
		minwait = min(minwait, max_wait);
		//cout << " maxd1: " << min_de / 60 << " maxd2: " << min_dl / 60 << " maxa1: " << min_ae / 60 << " maxa2: " << min_al / 60 << " maxxt1: " << minxt1 / 60 << " maxxt2: " << minxt2 / 60 << " maxwait: " << minwait / 60 << " maxt1: " << min_t1 / 60 << " maxt2: " << min_t2 / 60 << " maxdb1: " << maxdb1 / 60 << " maxdb2: " << maxdb2 / 60 << endl;
		//cout << "MaxRW " << min(min(min(min(min_ae, min_de), min_t1), minxt1), maxdb2) / 60 << " maxFW: " << min(min(min(min(min_al, min_dl), min_t2), minxt2), maxdb2) / 60 << " maxwait: " << minwait / 60 << endl;
		//cout << "not driving yet!\n";
		int UBs = 0, LBs = 0;
		if (min_al < 0 || min_dl < 0 || min_t2 < 0 || minxt2 < 0 || maxdb2 < 0) {// if the insertion of the stop made the solution infeasible (too much into the future)
			//cout <<" became infeasible bcs of stop insertion\n";
			maxFW = -min(min(min(min(min_al, min_dl), min_t2), minxt2), maxdb2); // correct with this value shift to the past 
			maxRW = min(min(min(min(min_ae, min_de), min_t1), minxt1), maxdb1);
			if (maxFW > maxRW) { // then infeasible
				FEAS = false;
			}
			else {
				// check bd feasibility
				if (trip != 0) {
					//adjust timetable between [maxFW,maxRW] in the past only
					int  shiftS = 0, shiftL = 0;
					if (FWpt < 0 && RWpt < 0) { //need to shift timetable to the past  at least shit maxFW, at most maxRW
						shiftS = -max(FWpt, RWpt); //most feasible shift 
						shiftL = -min(FWpt, RWpt); //least feasible shift 
						if (shiftS > maxRW || shiftL < maxFW) FEAS = false;
						else {
							UBs = min(shiftL, maxRW);//max to shift back 
							LBs = max(shiftS, maxFW);// min to shift back
							//cout << " ----> but can be fixed by shifting " << LBs / 60 << " to the past\n";
							for (j = 0; j < nBus; j++) {
								timetable[j] -= ((1 - pm) * LBs + UBs * pm);
							}
						}
					}
					else if (FWpt > 0 && RWpt > 0) { //cannot shift timetable to the future 
						FEAS = false;
					}
					else { //timetable already in the feasible region
						if (-RWpt < maxFW) FEAS = false;
						else {
							LBs = maxFW; //min to shift back 
							UBs = -RWpt; //max to shift back 
							//cout << " ----> but can be fixed by shifting " << LBs / 60 << " to the past\n";
							for (j = 0; j < nBus; j++) {
								timetable[j] -= ((1 - pm) * LBs + UBs * pm);
							}
						}
					}

				}
				else {
					//adjust timetable between [maxFW,maxRW] in the past only
					int  shiftS = 0, shiftL = 0;
					if (FWpt < 0 && RWpt < 0) { //need to shift timetable to the past  at least shit maxFW, at most maxRW
						shiftS = -max(FWpt, RWpt); //most feasible shift 
						shiftL = -min(FWpt, RWpt); //least feasible shift 
						if (shiftS > maxRW || shiftL < maxFW) FEAS = false;
						else {
							UBs = min(shiftL, maxRW);//max to shift back 
							LBs = max(shiftS, maxFW);// min to shift back
							//cout << " ----> but can be fixed by shifting " << LBs / 60 << " to the past\n";
							for (j = 0; j < nBus; j++) {
								timetable[j] -= ((1 - pm) * LBs + UBs * pm);
							}
						}
					}
					else if (FWpt > 0 && RWpt > 0) { //cannot shift timetable to the future at most maxFW
						FEAS = false;
					}
					else { //timetable already in the feasible region
						if (-RWpt < maxFW) FEAS = false;
						else {
							LBs = maxFW; //min to shift back 
							UBs = -RWpt; //max to shift back 
							//cout << " ----> but can be fixed by shifting " << LBs / 60 << " to the past\n";
							for (j = 0; j < nBus; j++) {
								timetable[j] -= ((1 - pm) * LBs + UBs * pm);
							}
						}
					}
				}
			}
		}
		else if (min_ae < 0 || min_de < 0 || min_t1 < 0 || minxt1 < 0 || maxdb1 < 0) {// if the insertion of the stop made the solution infeasible (too much into the past)
			//cout <<" became infeasible bcs of stop insertion\n";
			maxFW = min(min(min(min(min_al, min_dl), min_t2), minxt2), maxdb2); 
			maxRW = -min(min(min(min(min_ae, min_de), min_t1), minxt1), maxdb1);// correct with this value shift to the future
			if (maxFW < maxRW) { // then infeasible
				FEAS = false;
			}
			else {
				// check bd feasibility
				if (trip != 0) {
					//adjust timetable between [maxRW,maxFW] in the future only
					int  shiftS = 0, shiftL = 0;
					if (FWpt < 0 && RWpt < 0) { //cannot shift timetable to the past
						FEAS = false;
					}
					else if (FWpt > 0 && RWpt > 0) { //need to shift timetable to the past  at least shit maxFW, at most maxRW
						shiftS = max(FWpt, RWpt); //most feasible shift 
						shiftL = min(FWpt, RWpt); //least feasible shift 
						if (shiftS > maxFW || shiftL < maxRW) FEAS = false;
						else if (shiftL >= maxRW && shiftS > maxFW && shiftS <= minwait) {
							UBs = min(shiftL, minwait); // max to shift forwards
							LBs = max(shiftS, maxRW); // min to shift forwards
							for (j = in_i; j < nBus; j++) {
								timetable[j] += ((1 - pm) * LBs + UBs * pm);
							}
						}
						else {
							UBs = min(shiftL, maxFW);//max to shift back 
							LBs = max(shiftS, maxRW);// min to shift back
							//cout << " ----> but can be fixed by shifting " << LBs / 60 << " to the past\n";
							for (j = 0; j < nBus; j++) {
								timetable[j] += ((1 - pm) * LBs + UBs * pm);
							}
						}
					}
					else { //timetable already in the feasible region
						if (-FWpt < maxRW) FEAS = false;
						else {
							LBs = maxRW; //min to shift back 
							UBs = FWpt; //max to shift back 
							//cout << " ----> but can be fixed by shifting " << LBs / 60 << " to the past\n";
							for (j = 0; j < nBus; j++) {
								timetable[j] += ((1 - pm) * LBs + UBs * pm);
							}
						}
					}

				}
				else {
					//adjust timetable between [maxRW,maxFW] in the future only
					int  shiftS = 0, shiftL = 0;
					if (FWpt < 0 && RWpt < 0) { //cannot shift timetable to the past
						FEAS = false;
					}
					else if (FWpt > 0 && RWpt > 0) { //need to shift timetable to the past  at least shit maxFW, at most maxRW
						shiftS = max(FWpt, RWpt); //most feasible shift 
						shiftL = min(FWpt, RWpt); //least feasible shift 
						if (shiftS > maxFW || shiftL < maxRW) FEAS = false;
						else if (shiftL >= maxRW && shiftS > maxFW && shiftS <= minwait) {
							UBs = min(shiftL, minwait); // max to shift forwards
							LBs = max(shiftS, maxRW); // min to shift forwards
							for (j = in_i; j < nBus; j++) {
								timetable[j] += ((1 - pm) * LBs + UBs * pm);
							}
						}
						else {
							UBs = min(shiftL, maxFW);//max to shift back 
							LBs = max(shiftS, maxRW);// min to shift back
							//cout << " ----> but can be fixed by shifting " << LBs / 60 << " to the past\n";
							for (j = 0; j < nBus; j++) {
								timetable[j] += ((1 - pm) * LBs + UBs * pm);
							}
						}
					}
					else { //timetable already in the feasible region
						if (-FWpt < maxRW) FEAS = false;
						else {
							LBs = maxRW; //min to shift back 
							UBs = FWpt; //max to shift back 
							//cout << " ----> but can be fixed by shifting " << LBs / 60 << " to the past\n";
							for (j = 0; j < nBus; j++) {
								timetable[j] += ((1 - pm) * LBs + UBs * pm);
							}
						}
					}
				}
			}
		}
		else {
			maxFW = -min(min(min(min(min_al, min_dl), min_t2), minxt2), maxdb2); // correct with this value shift to the past 
			maxRW = min(min(min(min(min_ae, min_de), min_t1), minxt1), maxdb1);
			int shiftS = 0, shiftL = 0;
			if (FWpt < 0 && RWpt < 0) { //need to shift timetable to the past at most maxRW
				shiftS = -max(FWpt, RWpt); //most feasible shift 
				shiftL = -min(FWpt, RWpt); //least feasible shift 
				if (shiftS > maxRW) FEAS = false;
				else {
					UBs = min(shiftL, maxRW); // max to shift back
					LBs = shiftS; // min to shift back
					//cout << "Needs shifting " << LBs / 60 << " to the past\n";
					for (j = 0; j < nBus; j++) {
						timetable[j] -= ((1 - pm) * LBs + UBs * pm);
					}
				}
			}
			else if (FWpt > 0 && RWpt > 0) { //need to shift timetable to the future at most maxFW
				shiftS = min(FWpt, RWpt); //most feasible shift 
				shiftL = max(FWpt, RWpt); //least feasible shift 
				if (shiftS > maxFW) FEAS = false;
				else if (shiftS > maxFW && shiftS <= minwait) {
					UBs = min(shiftL, minwait); // max to shift forwards
					LBs = shiftS; // min to shift forwards
					for (j = in_i; j < nBus; j++) {
						timetable[j] += ((1 - pm) * LBs + UBs * pm);
					}
				}
				else {
					UBs = min(shiftL, maxFW); // max to shift forwards
					LBs = shiftS; // min to shift forwards
					//cout << "Needs shifting " << LBs / 60 << " to the future\n";
					for (j = 0; j < nBus; j++) {
						timetable[j] += ((1 - pm) * LBs + UBs * pm);
					}
				}
			}
			else { //timetable already in the feasible region
				UBs = min(-RWpt, maxRW);// max to shift forwards
				LBs = min(FWpt, maxFW);// max to shift backwards
				//cout << "does not need  shifting \n";
				// Here we can shift timetable BUT DONT HAVE TO
				if (pm > 0.5) {
					for (j = 0; j < nBus; j++) {
						timetable[j] += (UBs * (pm - 0.5) * 2);
					}
				}
				else if (pm <= 0.5 && pm > 0) {
					for (j = 0; j < nBus; j++) {
						timetable[j] -= (LBs * pm * 2);
					}
				}
			}

		}
	}
	if (FEAS) {
		//cout << "Passengers onboard " << p_onboard << endl;
		//Calculate added cost
		extracost = c2 * traveltimep[pt][s];
		if (pt < OG_R1)extracost += c3 * abs(OG_arrivals[pt] - timetable[nBus - 1]);
		else extracost += c3 * abs(OG_departures[pt - OG_R1] - timetable[in_i]);
		//cout << " cost: " << extracost << endl;
		extracost += c1 * (timetable[nBus - 1] - timetable[in_i]);
		//cout << " cost: " << extracost << endl;
		/*
		cout << "old tt: \n";
		for (int ii = 0; ii < b_Dsol[bus][trip].size(); ii++) {
			cout << b_Dsol[bus][trip][ii] / 60 << '\t';
		}
		cout << endl;
		cout << "new tt: \n";
		for (int ii = 0; ii < timetable.size(); ii++) {
			cout << timetable[ii] / 60 << '\t';
		}
		cout << endl;
		//*/
		for (int p = 0; p < OG_R; p++) {
			dst = -1;
			for (int ii = 0; ii < nBus; ii++) {
				if (b_ysol[p][0] == bus && b_ysol[p][1] == trip && b_ysol[p][2] == route[ii]) {
					dst = ii;
					break;
				}
			}
			if (dst != -1) {
				if (dst <= in_i) extracost += c1 * extratrav;
				if (p < OG_R1)extracost += c3 * (abs(OG_arrivals[p] - timetable[nBus - 1]) - abs(OG_arrivals[p] - b_Dsol[bus][trip][b_Dsol[bus][trip].size() - 1]));
				else {
					int dst0 = -1;
					for (int ii = 0; ii < b_xsol[bus][trip].size(); ii++) {
						if (b_ysol[p][0] == bus && b_ysol[p][1] == trip && b_ysol[p][2] == b_xsol[bus][trip][ii]) {
							dst0 = ii;
							break;
						}
					}
					extracost += c3 * (abs(OG_departures[p - OG_R1] - timetable[dst]) - abs(OG_departures[p - OG_R1] - b_Dsol[bus][trip][dst0]));
				}
			}
		}
		//cout << "******* Is FEASIBLE with stop " << s << " with cost = " << extracost;


		if (mincost > extracost) {
			mincost = extracost;
			b_route.clear();
			b_timetable.clear();
			for (j = 0; j < nBus; j++) {
				b_route.push_back(route[j]);
				b_timetable.push_back(timetable[j]);
			}
			b_stop = s;
		}
		//cout << endl;
	}
	//else cout << " NOT FEASIBLE with stop " << s << " and cost " <<  mincost << endl;
	i++;

	if (mincost == INT32_MAX) {
		//cout << "--> NOT FEASIBLE " << endl;
		return -1;
	}
	else {
		//cout << "******* Is BEST FEASIBLE with stop " << s << " with cost = " << mincost << endl;
		return mincost;
	}
}

inline double printpluscost(vector<vector<vector<int>>> b_xsol, vector<vector<vector<double>>> b_Dsol, int** b_ysol, int B, int OG_R, int OG_R1, int N, double** traveltimes, double** traveltimep, double* pickup,
	double* OG_departures, double* OG_arrivals, float c1, float c2, float c3, int OGxt, int d_ae, int d_al, int d_dl, int d_de, int dw, double penalty, double short_route, int** closestPS, int d_t, bool print, bool& FEAS) {
	int b, t, s, p;
	double cost = 0;
	int c_rejec = 0;
	FEAS = true;
	if (print) {
		cout << "XSOL (route)\n";
		for (b = 0; b < B; b++) {
			cout << " BUS " << b << endl;
			for (t = 0; t < b_xsol[b].size(); t++) {
				for (s = 0; s < b_xsol[b][t].size(); s++) {
					cout << b_xsol[b][t][s] << "\t";
				}
				cout << "\n";
			}
		}
	}
	if (print) cout << "DSOL (timetable)\n";
	for (b = 0; b < B; b++) {
		if (print)cout << " BUS " << b << endl;
		for (t = 0; t < b_Dsol[b].size(); t++) {
			for (s = 0; s < b_Dsol[b][t].size(); s++) {
				if (s == 0 && t != 0) {
					if (b_Dsol[b][t][s] + 10 < int(b_Dsol[b][t - 1][b_Dsol[b][t - 1].size() - 1] + short_route)) {
						FEAS = false;
						if (print)cout << round(b_Dsol[b][t][s] / 60 * 100) / 100 << "**\t";
					}
					else if (print) cout << round(b_Dsol[b][t][s] / 60) << "\t";
				}
				else if (print)cout << round(b_Dsol[b][t][s] / 60) << "\t";
			}
			if (print)cout << "\n";
		}
	}

	if (print)cout << "YSOL (assignments)\n";
	string ov = "";
	//Objective function value
	cost = 0;
	for (p = 0; p < OG_R; p++) {
		if (b_ysol[p][0] > -1) {
			b = b_ysol[p][0];
			t = b_ysol[p][1];
			s = b_ysol[p][2];
			if (p == OG_R1 && print) cout << endl;
			if (print)cout << "p_" << p << "\tb_" << b << " t_" << t << " s_" << s;
			//continue;
			//Walking for all passengers
			
			if (traveltimep[p][s] > dw) {
				if (print) cout << "\tW: " << round(traveltimep[p][s] / 60) << "**";
				FEAS = false;
			}
			else if (print) cout << "\tW: " << round(traveltimep[p][s] / 60);
			
			cost += c2 * traveltimep[p][s];
			bool in = false;
			//Travel for all passengers
			double ttimep = 0;
			int l;
			for (int i = 0; i < b_xsol[b][t].size() - 1; i++) {
				if (b_xsol[b][t][i] == s || in) {
					if (!in)l = i;
					in = true;
					cost += c1 * traveltimes[b_xsol[b][t][i]][b_xsol[b][t][i + 1]];
					ttimep += traveltimes[b_xsol[b][t][i]][b_xsol[b][t][i + 1]];
				}
			}
			if (s == N - 1) l = b_xsol[b][t].size() - 1;
			if (print)cout << "\tT: " << round(ttimep / 60);
			//waiting
			//cout <<"Passenger " << p<<" Stop " << s << " index ";
			//cout << endl << l << "  " << xk[b][t].size() << endl;

			if (abs(int(b_Dsol[b][t][l] - pickup[p])) > d_t) {
				FEAS = false;
				ov = "\tptp: " + to_string(int(round(pickup[p] / 60))) + " pt: " + to_string(int(round(b_Dsol[b][t][l] / 60))) + " --> Diff: " + to_string(int((b_Dsol[b][t][l] - pickup[p]) / 60 * 10) / 10) + "**\n";
			}
			else ov = "\tptp: " + to_string(int(round(pickup[p] / 60))) + " pt: " + to_string(int(round(b_Dsol[b][t][l] / 60))) + " --> Diff: " + to_string(int((b_Dsol[b][t][l] - pickup[p]) / 60 * 10) / 10) + "\n";

			if (p < OG_R1) {
				cost += c3 * abs(OG_arrivals[p] - b_Dsol[b][t].back());
				if (int(OG_arrivals[p] - b_Dsol[b][t].back()) > d_ae || int(b_Dsol[b][t].back() - OG_arrivals[p]) > d_al) {
					FEAS = false;
					if (print) cout << "\tDAT: " << round((OG_arrivals[p]) / 60) << " a: " << round(b_Dsol[b][t].back() / 60) << " --> Diff: " << round(b_Dsol[b][t].back() / 60 * 10) / 10 - round((OG_arrivals[p]) / 60 * 10) / 10 << "**" << ov;
				}
				else {
					if (print) cout << "\tDAT: " << round((OG_arrivals[p]) / 60) << " a: " << round(b_Dsol[b][t].back() / 60) << " --> Diff: " << round(b_Dsol[b][t].back() / 60 * 10) / 10 - round((OG_arrivals[p]) / 60 * 10) / 10 << ov;
				}
				
			}
			else {
				cost += c3 * abs(OG_departures[p - OG_R1] - b_Dsol[b][t][l]);
				if (int(OG_departures[p - OG_R1] - b_Dsol[b][t][l]) > d_de || int(b_Dsol[b][t][l] - OG_departures[p - OG_R1]) > d_dl) {
					FEAS = false;
					if (print)cout << "\tDDT: " << round((OG_departures[p - OG_R1]) / 60) << " d: " << round(b_Dsol[b][t][l] / 60) << " --> Diff: " << round(b_Dsol[b][t][l] / 60 * 10) / 10 - round((OG_departures[p - OG_R1]) / 60 * 10) / 10 << "**" << ov;
				}
				else {
					if (print)cout << "\tDDT: " << round((OG_departures[p - OG_R1]) / 60) << " d: " << round(b_Dsol[b][t][l] / 60) << " --> Diff: " << round(b_Dsol[b][t][l] / 60 * 10) / 10 - round((OG_departures[p - OG_R1]) / 60 * 10) / 10 << ov;
				}
			}
		}
		else if(closestPS[p][0] != N - 1) {
			if (print) {
				if (p < OG_R1) {
					cout << "\tPassenger " << p << " rejected, DAT=" << OG_arrivals[p] / 60 << " closest stop " << closestPS[p][0] << endl;
				}
				else {
					cout << "\tPassenger " << p << " rejected, DDT=" << OG_departures[p - OG_R1] / 60 << " closest stop " << closestPS[p][0] << endl;
				}
			}
			c_rejec++;
			cost += penalty;
		}
		else {
			cost += c2 * traveltimep[p][N - 1];
			if (print) {
				cout << "\tPassenger " << p << " just walks to the desination at stop " << N - 1 << " for " << traveltimep[p][N - 1] / 60 << " min\n";
			}
		}
	}
	
	vector<vector<double>> FC1(N);
	if (print) cout << "\n----------  Time between bus departures (min) ----------- \n";
	for (int i = 0; i < N; i++) {
		for (int b = 0; b < B; b++) {
			for (int t = 0; t < b_xsol[b].size(); t++) {
				for (int l = 0; l < b_xsol[b][t].size(); l++) {
					if (b_xsol[b][t][l] == i)FC1[i].push_back(b_Dsol[b][t][l]);
				}
			}
		}
		std::sort(FC1[i].begin(), FC1[i].begin() + FC1[i].size());
		if (print) cout << "m_" << i << "->\t";
		for (int j = 1; j < FC1[i].size(); j++) {
			if (int(FC1[i][j] - FC1[i][j - 1]) > OGxt) {
				FEAS = false;
				if (print)cout << round((FC1[i][j] - FC1[i][j - 1]) / 60 * 100) / 100 << "**\t";
			}
			else {
				if (print)cout << round((FC1[i][j] - FC1[i][j - 1]) / 60) << "\t";
			}
		}
		if (print)cout << endl;
	}
	if (print) {
		cout << "-------------------- COST (with penalty): " << int(cost) << endl;
		if (c_rejec != OG_R) cout << "-------------------- Average cost p.p. in the service: " << int((cost - penalty * c_rejec) / (OG_R - c_rejec)) << endl;
		cout << "\t" << c_rejec << "/" << OG_R << " passengers rejected\n";
	}
	
	return cost;
}

inline double Improvement(int B, int N, int M, int S, int OG_R, int OG_R1, int OG_R2, int OGxt, int C_OG, int d_dl, int d_de, int d_ae, int d_al, int d_t, double short_route, double* pickup, double* OG_departures, double* OG_arrivals, 
	int* best_route, double** traveltimes, double** traveltimep, int** closestPS, int dw,  vector<vector<vector<int>>>& b_xsol, vector<vector<vector<double>>>& b_Dsol, int**& b_ysol, int max_wait, 
	float c1, float c2, float c3, double endtime, int* b_trips, double* b_bd, double* b_freqN, int* pickupstops, double penalty, int N_it, double* timestamp, double max_runtime) {
	// INFO: ysol, xsol and Dsol are only the trips in the future wrt to timestamp
	cout << "  ++++++++++++++++++++++ BEGIN IMPROVEMENT +++++++++++++++++++++++\n";
	double start_time = clock(), elapsed_time =0;
	int i, j, b, s, p, l, t, p_it, X1, X2;

	double min_costt =  c2 * dw / 4 + c1 * short_route / 2;

	double currtime = 0;
	bool fixed = true;
	vector<int> P1, P2;
	double* tpickup = new double[OG_R];
	int* tpickupstops = new int[OG_R];
	bool isFEAS = true;

	for (i = 0; i < OG_R; i++) {
		tpickup[i] = -1;
		tpickupstops[i] = -1;
		//cout << " ysol0: " << b_ysol[i][0] << endl;
		if ((b_ysol[i][0] == -3 || b_ysol[i][0] == -2) && i < OG_R1) P1.push_back(i);
		if ((b_ysol[i][0] == -3 || b_ysol[i][0] == -2) && i >= OG_R1) P2.push_back(i);
		//if (b_ysol[i][0] == -2) cout << "passenger " << i << " is new  and pick-up time: " << round(tpickup[i] / 60) << " stop: " << tpickupstops[i] << endl;
		if (b_ysol[i][0] == -3) {
			tpickup[i] = pickup[i];
			tpickupstops[i] = pickupstops[i];
			//cout << "passenger " << i << " is OLD and pick-up time: " << round(tpickup[i] / 60) << " stop: " << tpickupstops[i] << endl;
		}
	}
	
	int R1 = P1.size();
	int R2 = P2.size();
	int R = R1 + R2;
	float* pm_tt = new float[OG_R];
	float* pm_acc = new float[OG_R];
	float pm_xt = 0.5;
	
	cout << "\twith " << R  << " passengers still without a fixed planning\n";
	int p1 = 0, p2 = 0, cp1 = 0, cp2 = 0;

	int* indexpt = new int[R];
	double* temparrivals = new double[R];
	
	vector<double> timetable;
	vector<int> route;
	double cost = 0, tt = 0, end_cost = INT32_MAX, temp_cost = 0;
	vector<vector<vector<double>>> FC(N);
	vector<vector<vector<int>>> xsol(B);
	vector<vector<vector<double>>> Dsol(B);
	vector<vector<vector<int>>> OGxsol(B);
	vector<vector<vector<double>>> OGDsol(B);
	for (b = 0; b < B; b++) {
		X1 = b_xsol[b].size();
		for (t = 0; t < X1; t++) {
			X2 = b_xsol[b][t].size();
			vector < int > x2;
			vector < double > D2;
			for (l = 0; l < X2; l++) {
				x2.push_back(b_xsol[b][t][l]);
				D2.push_back(b_Dsol[b][t][l]);
			}
			OGxsol[b].push_back(x2);
			OGDsol[b].push_back(D2);
		}
	}
	int** ysol = new int* [OG_R];
	int** OGysol = new int* [OG_R];
	for (i = 0; i < OG_R; i++) {
		ysol[i] = new int[3];
		OGysol[i] = new int[3];
		OGysol[i][0] = b_ysol[i][0];
		OGysol[i][1] = b_ysol[i][1];
		OGysol[i][2] = b_ysol[i][2];
	}
	int* trips = new int[B];
	double* bd = new double[B];
	double* freqN = new double[N];
	default_random_engine r_gen;
	uniform_real_distribution<double> unif1(0.5, 1.0);
	uniform_real_distribution<double> unif0(0.25, 1.0);
	uniform_real_distribution<double> unif2(0.0, 1.0);
	r_gen.seed(528200);
	
	for (int it = 0; it < N_it; it++) {
		elapsed_time = (double)(clock() - start_time) / CLK_TCK;
		if (elapsed_time > max_runtime) break;
		//construction parameters 
		pm_xt = unif0(r_gen);
		for (i = 0; i < OG_R; i++) {
			pm_tt[i] = unif2(r_gen);
			pm_acc[i] = unif1(r_gen);
		}

		//intialize other parameters
		for (b = 0; b < B; b++) {
			xsol[b].clear();
			Dsol[b].clear();
			X1 = OGxsol[b].size();
			for (t = 0; t < X1; t++) {
				X2 = OGxsol[b][t].size();
				vector < int > x2;
				vector < double > D2;
				for (l = 0; l < X2; l++) {
					x2.push_back(OGxsol[b][t][l]);
					D2.push_back(OGDsol[b][t][l]);
				}
				xsol[b].push_back(x2);
				Dsol[b].push_back(D2);
			}
		}
		for (i = 0; i < OG_R; i++) {
			ysol[i][0] = OGysol[i][0];
			ysol[i][1] = OGysol[i][1];
			ysol[i][2] = OGysol[i][2];
		}

		for (i = 0; i < B; i++) {
			bd[i] = b_bd[i];
			trips[i] = b_trips[i];
		}
		for (i = 0; i < N; i++) {
			freqN[i] = b_freqN[i];
		}

		currtime = 0;
		fixed = true;
		isFEAS = true;
		p1 = 0, p2 = 0, cp1 = 0, cp2 = 0;

		for (p = 0; p < R1; p++) {
			indexpt[p] = P1[p];
			temparrivals[p] = OG_arrivals[P1[p]];
		}
		for (p = 0; p < R2; p++) {
			indexpt[p + R1] = P2[p];
			temparrivals[p + R1] = OG_departures[P2[p]] + traveltimes[closestPS[P2[p]][0]][N - 1];
		}
		quickSort(indexpt, temparrivals, 0, R - 1);

		cost = 0, tt = 0;
		//departures at mandatory stops
		for (i = 0; i < N; i++) {
			FC[i].clear();
			for (b = 0; b < B; b++) {
				X1 = xsol[b].size();
				for (t = 0; t < X1; t++) {
					X2 = xsol[b][t].size();
					for (l = 0; l < X2; l++) {
						if (xsol[b][t][l] == i)FC[i].push_back({ Dsol[b][t][l], float(b), float(t) });
					}
				}
			}
		}

		double t_xt = max(int(OGxt * pm_xt), 8 * 60);
		isFEAS = true;
		while (currtime < endtime) {

			//determine next bus 
			b = iMin(bd, B);
			//cout << "\n   +++++++++++++ BUS " << b << " on trip " << trips[b] << " bd: " << round(bd[b] / 60) << endl;
			//printpluscost(b_xsol, b_Dsol, b_ysol, B, OG_R, OG_R1, N, traveltimes, traveltimep, tpickup, OG_departures, OG_arrivals, c1, c2, c3, OGxt, d_ae, d_al, d_dl, d_de, dw, penalty, short_route, closestPS, d_t, true);

			//initialize timetable and route 
			route.clear();
			timetable.clear();
			if (p1 + p2 < R) timetable.push_back(max(freqN[0] + t_xt, bd[b]));
			else timetable.push_back(max(freqN[0] + t_xt, bd[b]));
			route.push_back(0);
			tt = -1;
			for (i = 1; i < N; i++) {
				route.push_back(i);
				timetable.push_back(timetable[i - 1] + traveltimes[i - 1][i]);
				if (freqN[i] != -1 && timetable[i] - freqN[i] > tt)tt = timetable[i] - freqN[i];
			}

			if (tt > OGxt) {
				for (i = 0; i < N; i++) {
					timetable[i] -= (tt - OGxt);
				}
			}

			if (timetable[0] < bd[b]) {
				//cout << " INFEASIBLE for xt\n";
				isFEAS = false;
				break;
			}
			xsol[b].push_back(route);
			Dsol[b].push_back(timetable);
			//intialize capacities
			cp1 = cp2 = 0;
			//loop on all available passegners for this bus on this trip
			p_it = 0;
			while (cp1 + cp2 < C_OG && p_it < R && p1 + p2 < R) {
				//cout << "lets start adding passengers to this bus\n";
				if (indexpt[p_it] != -1) p = indexpt[p_it]; // determine the next passenger and type of request
				else {
					p_it++;
					continue;
				}
				if (ysol[p][0] == -3) fixed = true;
				else fixed = false;
				//cout << "\n\tLet's try passenger " << p << endl;
				//cout << "pm " << pm_tt[p] << " p " << p << " p_it " << p_it << endl;
				// determine the cost of assigning passenger p on bus b antrip trips[b], if feasible
				if (!fixed) cost = insertstop(route, timetable, N, M, S, OG_R, OG_R1, OG_R2, OGxt, d_dl, d_de, d_ae, d_al, d_t, short_route, tpickup, OG_departures, OG_arrivals, best_route,
					traveltimes, traveltimep, closestPS, dw, FC, xsol, Dsol, ysol, p, b, trips[b], timestamp[p], false, -1, -1, max_wait, s, c1, c2, c3, pm_tt[p]);
				else cost = insertfixedstop(route, timetable, N, M, S, OG_R, OG_R1, OG_R2, OGxt, d_dl, d_de, d_ae, d_al, d_t, short_route, tpickup, OG_departures, OG_arrivals, best_route, traveltimes,
					traveltimep, dw, FC, xsol, Dsol, ysol, p, b, trips[b], s, c1, c2, c3, tpickupstops[p], pm_tt[p], max_wait, timestamp[p]);

				if (cost != -1 && (cost - min_costt) / cost <= pm_acc[p]) {//passengers feasible and assigned 
					if (p < OG_R1) {
						cp1++;
						p1++;
					}
					else {
						cp2++;
						p2++;
					}
					//cout << "\t==> Passenger " << p << " assigned to Bus " << b << " on trip " << trips[b] << endl;
					indexpt[p_it] = -1;
					xsol[b][trips[b]].clear();
					Dsol[b][trips[b]].clear();
					X1 = route.size();
					for (j = 0; j < X1; j++) {
						xsol[b][trips[b]].push_back(route[j]);
						Dsol[b][trips[b]].push_back(timetable[j]);
					}

					ysol[p][0] = b;
					ysol[p][1] = trips[b];
					ysol[p][2] = s;
				}
				p_it++;

			}
			//update departures at mandatory stops
			for (i = 0; i < N; i++) {
				FC[i].clear();
				for (j = 0; j < B; j++) {
					X1 = xsol[j].size();
					for (t = 0; t < X1; t++) {
						X2 = xsol[j][t].size();
						for (l = 0; l < X2; l++) {
							if (xsol[j][t][l] == i)FC[i].push_back({ Dsol[j][t][l], float(j), float(t) });
						}
					}
				}
			}

			//update freqN
			X1 = xsol[b][trips[b]].size();
			for (i = 0; i < X1; i++) {
				if (xsol[b][trips[b]][i] < N && (freqN[xsol[b][trips[b]][i]] < Dsol[b][trips[b]][i] || freqN[xsol[b][trips[b]][i]] - Dsol[b][trips[b]][i] > OGxt)) {
					freqN[xsol[b][trips[b]][i]] = Dsol[b][trips[b]][i];
					//t++;
				}
			}

			currtime = Dsol[b][trips[b]].back();
			bd[b] = currtime + short_route;
			trips[b]++;
		}

		if (isFEAS) {
			temp_cost = printpluscost(xsol, Dsol, ysol, B, OG_R, OG_R1, N, traveltimes, traveltimep, pickup, OG_departures, OG_arrivals, c1, c2, c3, OGxt,
				d_ae, d_al, d_dl, d_de, dw, penalty, short_route, closestPS, d_t, false, isFEAS);
			if (!isFEAS || p1 + p2 < R) temp_cost = INT32_MAX;
		}
		else temp_cost = INT32_MAX;

		if (temp_cost < end_cost) {
			end_cost = temp_cost;
			//cout << " new best cost: " << end_cost << endl;
			for (b = 0; b < B; b++) {
				b_xsol[b].clear();
				b_Dsol[b].clear();
				X1 = xsol[b].size();
				//cout << "bus " << b + 1 << " nr trips " << X1 << endl;
				for (t = 0; t < X1; t++) {
					X2 = xsol[b][t].size();
					vector < int > x2;
					vector < double > D2;
					for (l = 0; l < X2; l++) {
						x2.push_back(xsol[b][t][l]);
						D2.push_back(Dsol[b][t][l]);
					}
					b_xsol[b].push_back(x2);
					b_Dsol[b].push_back(D2);
				}
			}
			for (i = 0; i < OG_R; i++) {
				b_ysol[i][0] = ysol[i][0];
				b_ysol[i][1] = ysol[i][1];
				b_ysol[i][2] = ysol[i][2];
			}

		}
	}
	
	//remove memory 
	delete []indexpt;
	delete []temparrivals;
	delete []tpickup;
	delete []tpickupstops;
	delete []pm_tt;
	delete []pm_acc;
	for (i = 0; i < OG_R; i++) {
		delete []ysol[i];
		delete []OGysol[i];
	}
	delete []ysol;
	delete []OGysol;
	delete []freqN;
	delete []bd;
	delete []trips;
	cout << "  ----------------------------  END IMPROVEMNT ----------------------------- with end_cost= " << end_cost << endl;
	return end_cost;
}

inline double StaticOpt(int B, int N, int M, int S, int OG_R, int OG_R1, int OG_R2, int OGxt, int C_OG, int d_dl, int d_de, int d_ae, int d_al, int d_t, double short_route, double* pickup, double* OG_departures, double* OG_arrivals,
	int* best_route, double** traveltimes, double** traveltimep, int** closestPS, int dw, vector<vector<vector<int>>>& b_xsol, vector<vector<vector<double>>>& b_Dsol, int**& b_ysol, int max_wait,
	float c1, float c2, float c3, double endtime, int* b_trips, double* b_bd, double* b_freqN, int* pickupstops, double penalty, int N_it, double T_min, int lam, double nph, double* timestamp) {
	// INFO: ysol, xsol and Dsol are only the trips in the future wrt to timestamp
	cout << "  ++++++++++++++++++++++ BEGIN IMPROVEMENT +++++++++++++++++++++++\n";
	int i, j, b, s, p, l, t, p_it, X1, X2;

	double min_costt = c2 * dw / 4 + c1 * short_route / 2;

	double currtime = 0;
	bool fixed = true;
	vector<int> P1, P2;
	double* tpickup = new double[OG_R];
	int* tpickupstops = new int[OG_R];
	bool isFEAS = true;

	for (i = 0; i < OG_R; i++) {
		tpickup[i] = -1;
		tpickupstops[i] = -1;
		//cout << " ysol0: " << b_ysol[i][0] << endl;
		if ((b_ysol[i][0] == -3 || b_ysol[i][0] == -2) && i < OG_R1) P1.push_back(i);
		if ((b_ysol[i][0] == -3 || b_ysol[i][0] == -2) && i >= OG_R1) P2.push_back(i);
		//if (b_ysol[i][0] == -2) cout << "passenger " << i << " is new  and pick-up time: " << round(tpickup[i] / 60) << " stop: " << tpickupstops[i] << endl;
		if (b_ysol[i][0] == -3) {
			tpickup[i] = pickup[i];
			tpickupstops[i] = pickupstops[i];
			//cout << "passenger " << i << " is OLD and pick-up time: " << round(tpickup[i] / 60) << " stop: " << tpickupstops[i] << endl;
		}
	}

	int R1 = P1.size();
	int R2 = P2.size();
	int R = R1 + R2;
	cout << "\twith " << R << " passengers still without a fixed planning\n";
	cout << "d_t: " << d_t / 60 << endl;
	int p1 = 0, p2 = 0, cp1 = 0, cp2 = 0;

	int* indexpt = new int[R];
	double* temparrivals = new double[R];

	vector<double> timetable;
	vector<int> route;
	double cost = 0, tt = 0, end_cost = INT32_MAX, temp_cost = 0, bb_cost = end_cost;
	vector<vector<vector<double>>> FC(N);
	vector<vector<vector<int>>> xsol(B);
	vector<vector<vector<double>>> Dsol(B);
	vector<vector<vector<int>>> OGxsol(B);
	vector<vector<vector<double>>> OGDsol(B);
	for (b = 0; b < B; b++) {
		X1 = b_xsol[b].size();
		for (t = 0; t < X1; t++) {
			X2 = b_xsol[b][t].size();
			vector < int > x2;
			vector < double > D2;
			for (l = 0; l < X2; l++) {
				x2.push_back(b_xsol[b][t][l]);
				D2.push_back(b_Dsol[b][t][l]);
			}
			OGxsol[b].push_back(x2);
			OGDsol[b].push_back(D2);
		}
	}
	int** ysol = new int* [OG_R];
	int** OGysol = new int* [OG_R];
	for (i = 0; i < OG_R; i++) {
		ysol[i] = new int[3];
		OGysol[i] = new int[3];
		OGysol[i][0] = b_ysol[i][0];
		OGysol[i][1] = b_ysol[i][1];
		OGysol[i][2] = b_ysol[i][2];
	}
	int* trips = new int[B];
	double* bd = new double[B];
	double* freqN = new double[N];
	default_random_engine r_gen;
	

	float* pm_tt = new float[OG_R];
	float* pm_acc = new float[OG_R];
	float pm_xt = 0.5;

	float* b_pm_tt = new float[OG_R];
	float* b_pm_acc = new float[OG_R];
	float b_pm_xt = 0.5;
	for (i = 0; i < OG_R; i++) {
		b_pm_tt[i] = 0;
		b_pm_acc[i] = 1;
	}

	double D_E = 0, T = T_min;
	int del = 0;
	uniform_real_distribution<double> r(0, 1);
	for (int it = 0; it < N_it; it++) {

		normal_distribution<double> dp1(b_pm_xt, nph); 
		pm_xt = dp1(r_gen);
		if (pm_xt > 1) pm_xt = 1;
		else if (pm_xt < 0.5) pm_xt = 0.5;
		//while (pm_xt > 1 || pm_xt < 0.5)pm_xt = dp1(r_gen);
		for (i = 0; i < OG_R; i++) {
			normal_distribution<double> dp2(b_pm_tt[i], nph);
			normal_distribution<double> dp3(b_pm_acc[i], nph);
			pm_tt[i] = dp2(r_gen);
			if(pm_tt[i] > 0.85) pm_tt[i] = 0.85;
			else if(pm_tt[i] < 0) pm_tt[i] = 0;
			pm_acc[i] = dp3(r_gen);
			if (pm_acc[i] > 1) pm_acc[i] = 1;
			else if (pm_acc[i] < 0.4) pm_acc[i] = 0.4;
		}

		//intialize other parameters
		for (b = 0; b < B; b++) {
			xsol[b].clear();
			Dsol[b].clear();
			X1 = OGxsol[b].size();
			for (t = 0; t < X1; t++) {
				X2 = OGxsol[b][t].size();
				vector < int > x2;
				vector < double > D2;
				for (l = 0; l < X2; l++) {
					x2.push_back(OGxsol[b][t][l]);
					D2.push_back(OGDsol[b][t][l]);
				}
				xsol[b].push_back(x2);
				Dsol[b].push_back(D2);
			}
		}
		for (i = 0; i < OG_R; i++) {
			ysol[i][0] = OGysol[i][0];
			ysol[i][1] = OGysol[i][1];
			ysol[i][2] = OGysol[i][2];
		}

		for (i = 0; i < B; i++) {
			bd[i] = b_bd[i];
			trips[i] = b_trips[i];
		}
		for (i = 0; i < N; i++) {
			freqN[i] = b_freqN[i];
		}

		currtime = 0;
		fixed = true;
		isFEAS = true;
		p1 = 0, p2 = 0, cp1 = 0, cp2 = 0;

		for (p = 0; p < R1; p++) {
			indexpt[p] = P1[p];
			temparrivals[p] = OG_arrivals[P1[p]];
		}
		for (p = 0; p < R2; p++) {
			indexpt[p + R1] = P2[p];
			temparrivals[p + R1] = OG_departures[P2[p]] + traveltimes[closestPS[P2[p]][0]][N - 1];
		}
		quickSort(indexpt, temparrivals, 0, R - 1);

		cost = 0, tt = 0;
		//departures at mandatory stops
		for (i = 0; i < N; i++) {
			FC[i].clear();
			for (b = 0; b < B; b++) {
				X1 = xsol[b].size();
				for (t = 0; t < X1; t++) {
					X2 = xsol[b][t].size();
					for (l = 0; l < X2; l++) {
						if (xsol[b][t][l] == i)FC[i].push_back({ Dsol[b][t][l], float(b), float(t) });
					}
				}
			}
		}

		while (currtime < endtime) {

			//determine next bus 
			b = iMin(bd, B);
			//cout << "\n   +++++++++++++ BUS " << b << " on trip " << trips[b] << " bd: " << round(bd[b] / 60) << endl;
			//printpluscost(b_xsol, b_Dsol, b_ysol, B, OG_R, OG_R1, N, traveltimes, traveltimep, tpickup, OG_departures, OG_arrivals, c1, c2, c3, OGxt, d_ae, d_al, d_dl, d_de, dw, penalty, short_route, closestPS, d_t, true);

			//initialize timetable and route 
			route.clear();
			timetable.clear();
			if (p1 + p2 < R) timetable.push_back(max(freqN[0] + OGxt * pm_xt, bd[b]));
			else timetable.push_back(max(freqN[0] + OGxt * pm_xt, bd[b]));
			route.push_back(0);
			tt = -1;
			for (i = 1; i < N; i++) {
				route.push_back(i);
				timetable.push_back(timetable[i - 1] + traveltimes[i - 1][i]);
				if (freqN[i] != -1 && timetable[i] - freqN[i] > tt)tt = timetable[i] - freqN[i];
			}

			if (tt > OGxt) {
				for (i = 0; i < N; i++) {
					timetable[i] -= (tt - OGxt);
				}
			}

			if (timetable[0] < bd[b]) {
				//cout << " INFEASIBLE for xt\n";
				break;
			}
			xsol[b].push_back(route);
			Dsol[b].push_back(timetable);
			//intialize capacities
			cp1 = cp2 = 0;
			//loop on all available passegners for this bus on this trip
			p_it = 0;
			while (cp1 + cp2 < C_OG && p_it < R && p1 + p2 < R) {
				//cout << "lets start adding passengers to this bus\n";
				if (indexpt[p_it] != -1) p = indexpt[p_it]; // determine the next passenger and type of request
				else {
					p_it++;
					continue;
				}
				if (ysol[p][0] == -3) fixed = true;
				else fixed = false;
				//if (fixed) cout << " whaaat\n";
				//cout << "\n\tLet's try passenger " << p << endl;
				//cout << "pm " << pm_tt[p] << " p " << p << " p_it " << p_it << endl;
				// determine the cost of assigning passenger p on bus b antrip trips[b], if feasible
				if (!fixed) cost = insertstop(route, timetable, N, M, S, OG_R, OG_R1, OG_R2, OGxt, d_dl, d_de, d_ae, d_al, d_t, short_route, tpickup, OG_departures, OG_arrivals, best_route,
					traveltimes, traveltimep, closestPS, dw, FC, xsol, Dsol, ysol, p, b, trips[b], timestamp[p], false, -1, -1, max_wait, s, c1, c2, c3, pm_tt[p]);
				else cost = insertfixedstop(route, timetable, N, M, S, OG_R, OG_R1, OG_R2, OGxt, d_dl, d_de, d_ae, d_al, d_t, short_route, tpickup, OG_departures, OG_arrivals, best_route, traveltimes,
					traveltimep, dw, FC, xsol, Dsol, ysol, p, b, trips[b], s, c1, c2, c3, tpickupstops[p], pm_tt[p], max_wait, timestamp[p]);

				if (cost != -1 && (cost - min_costt) / cost <= pm_acc[p]) {//passengers feasible and assigned 
					if (p < OG_R1) {
						cp1++;
						p1++;
					}
					else {
						cp2++;
						p2++;
					}
					//cout << "\t==> Passenger " << p << " assigned to Bus " << b << " on trip " << trips[b] << endl;
					indexpt[p_it] = -1;
					xsol[b][trips[b]].clear();
					Dsol[b][trips[b]].clear();
					X1 = route.size();
					for (j = 0; j < X1; j++) {
						xsol[b][trips[b]].push_back(route[j]);
						Dsol[b][trips[b]].push_back(timetable[j]);
					}

					ysol[p][0] = b;
					ysol[p][1] = trips[b];
					ysol[p][2] = s;
				}
				p_it++;

			}
			//update departures at mandatory stops
			for (i = 0; i < N; i++) {
				FC[i].clear();
				for (j = 0; j < B; j++) {
					X1 = xsol[j].size();
					for (t = 0; t < X1; t++) {
						X2 = xsol[j][t].size();
						for (l = 0; l < X2; l++) {
							if (xsol[j][t][l] == i)FC[i].push_back({ Dsol[j][t][l], float(j), float(t) });
						}
					}
				}
			}

			//update freqN
			X1 = xsol[b][trips[b]].size();
			for (i = 0; i < X1; i++) {
				if (xsol[b][trips[b]][i] < N && (freqN[xsol[b][trips[b]][i]] < Dsol[b][trips[b]][i] || freqN[xsol[b][trips[b]][i]] - Dsol[b][trips[b]][i] > OGxt)) {
					freqN[xsol[b][trips[b]][i]] = Dsol[b][trips[b]][i];
					//t++;
				}
			}

			currtime = Dsol[b][trips[b]].back();
			bd[b] = currtime + short_route;
			trips[b]++;
		}

		temp_cost = printpluscost(xsol, Dsol, ysol, B, OG_R, OG_R1, N, traveltimes, traveltimep, pickup, OG_departures, OG_arrivals, c1, c2, c3, OGxt,
			d_ae, d_al, d_dl, d_de, dw, penalty, short_route, closestPS, d_t, false, isFEAS);
		if (!isFEAS || p1 + p2 < R) continue;

		D_E = temp_cost - end_cost;
		if (D_E < 0) {
			end_cost = temp_cost;
			if (bb_cost > end_cost) {
				cout << " new best cost: " << end_cost << endl;
				bb_cost = end_cost;
				for (b = 0; b < B; b++) {
					b_xsol[b].clear();
					b_Dsol[b].clear();
					X1 = xsol[b].size();
					for (t = 0; t < X1; t++) {
						X2 = xsol[b][t].size();
						vector < int > x2;
						vector < double > D2;
						for (l = 0; l < X2; l++) {
							x2.push_back(xsol[b][t][l]);
							D2.push_back(Dsol[b][t][l]);
						}
						b_xsol[b].push_back(x2);
						b_Dsol[b].push_back(D2);
					}
				}
				for (i = 0; i < OG_R; i++) {
					b_ysol[i][0] = ysol[i][0];
					b_ysol[i][1] = ysol[i][1];
					b_ysol[i][2] = ysol[i][2];
				}
			}
			b_pm_xt = pm_xt;
			for (i = 0; i < OG_R; i++) {
				b_pm_tt[i] = pm_tt[i];
				b_pm_acc[i] = pm_acc[i];
			}
		}
		else if (exp(-D_E / T) >= r(r_gen)) {
			end_cost = temp_cost;
			b_pm_xt = pm_xt;
			for (i = 0; i < OG_R; i++) {
				b_pm_tt[i] = pm_tt[i];
				b_pm_acc[i] = pm_acc[i];
			}
		}

		T = T_min + lam * (log(1 + del));
		if (D_E > 0) del++;
		else if (D_E < 0)del = 0;
	}

	//remove memory 
	delete []indexpt;
	delete []temparrivals;
	delete []tpickup;
	delete []tpickupstops;
	delete []pm_tt;
	delete []pm_acc;
	delete []b_pm_tt;
	delete []b_pm_acc;
	for (i = 0; i < OG_R; i++) {
		delete []ysol[i];
		delete []OGysol[i];
	}
	delete []ysol;
	delete []OGysol;
	delete []freqN;
	delete []bd;
	delete []trips;
	cout << "  ----------------------------  END IMPROVEMNT ----------------------------- with end_cost= " << bb_cost << endl;
	return bb_cost;
}
	
int main() {
	int i, j, k, b, t, p, l, s;
	for(int iruns =0;iruns < 5;iruns++){
	bool inst_gen = 1;
	int instance = iruns +1, experiment =0;
	bool SOpt = 1;
	string igen = "";
	if (inst_gen) igen = "Antwerp/";
	int N_it = 30000;
	ofstream inst("data/input/Instance_ANT_" + to_string(instance) + ".txt");
	inst << "---------- Weight factors of the objective function -------- " << endl << endl;
	//WEIGHT FACTORS--------------------------------------------------------------------------
	float c1 = 0.33f;
	inst << "c1: " << c1 << " \t (For travel-time of the buses)" << endl;
	float c2 = 0.33f;
	inst << "c2: " << c2 << " \t (For walking time of the passengers)" << endl;
	float c3 = 1 - c1 - c2;
	inst << "c3: " << c3 << " \t (For the absolute difference in desired arrival time and actual arrival time of the passengers)" << endl;
	inst << endl;
	inst << "---------------------- Parameters -------------------------- " << endl << endl;
	//Define parameters-----------------------------------------------------------------------
	//const double beforehand = 0.3; // percentage of passengers that requestred a service before the start of operation
	const int B = 18; // number of buses available
	inst << "Number of buses: " << B << endl;
	const int N = 8; // number of mandatory stations
	inst << "Number of mandatory bus stops: " << N << endl;
	const int M = 8; // number of stations in cluster
	inst << "Number optional bus stops per cluster: " << M << " \n --> One cluster between each mandatory stop: " + to_string((N - 1) * M) + " optional stops in total" << endl;
	const int S = (N - 1) * M + N; // amount of Stations
	inst << "Total number of bus stops: " << S << endl;
	const int OG_R = 100; // number of passenger requests
	const int OG_R1 = int(OG_R / 2), OG_R2 = OG_R - OG_R1;
	const int R = 3; // number of passengers that requestred a service before the start of operation
	const int R1 = R / 2, R2 = R1;
	//const int R = OG_R; // number of passenger requests
	//int R1 = int(R / 2), R2 = R - R1;
	//const int SO = int(beforehand * R); //number of passengers that requestred a service before the start of operation
	inst << "Number of passenger requests: " << OG_R << endl;
	//double pm1 = 0, pm2 = 0, pm3 = 1, fpm = 0;
	bool isFEAS = true;

	int C_OG = 20;
	int C = C_OG; // Bus capcity
	inst << "Bus capacity: " << C_OG << " passengers" << endl;
	const int TS = 3600 * 4.2;
	inst << "Planning horizon: " << TS << "s" << endl;
	int OGxt = 60 * 20;
	inst << "Minimum time between two buses departing from a mandatory stop: " << OGxt << "s" << endl;
	const float pspeed = 1.0f; // passengers speed in meter per scond
	const float bspeed = 40.0f / 3.6f; //bus speed in m/s
	const int dw = 20 * 60; // threshold of individual walking time in sec
	inst << "Maximum walking time for any passenger: " << dw << " seconds" << endl;
	const int d_dl = 10 * 60;
	const int d_de = 10 * 60;
	const int d_ae = 15 * 60;
	const int d_al = 10 * 60;
	const int d_t = 10 * 60;
	const int max_wait = 5 * 60; // max amount of time a bus can wait
	const int max_request_wait = 5 * 60; // max amount of time for passengers to recieve an answer after request
	inst << "Maximum amount of time a passenger can arrive too early w.r.t. their desired arrival time: " << d_ae << " seconds" << endl;
	inst << "Maximum amount of time a passenger can arrive too late w.r.t. their desired arrival time: " << d_al << " seconds" << endl;
	inst << "Maximum amount of time a passenger can depart too early w.r.t. their desired departure time: " << d_de << " seconds" << endl;
	inst << "Maximum amount of time a passenger can depart too late w.r.t. their desired departure time: " << d_dl << " seconds" << endl;
	inst << "Maximum amount of time a passenger can depart too late or too early w.r.t. their promised pick-up time: " << d_t << " seconds" << endl;
	inst << "Maximum amount of time a bus can be idle to accomodate a request: " << max_wait << " seconds" << endl;
	inst << "Maximum amount of time a passenger has to wait to receive a response after sending a request for transportation: " << max_request_wait << " seconds" << endl;
	//const int M0 = 10000; // Big M 

	// Read in locations 
	double** passengers = new double* [OG_R];
	for (i = 0; i < OG_R; i++) {
		passengers[i] = new double[2];
	}
	ifstream filep("data/input/" + igen + "passengers" + to_string(OG_R) + ".txt");
	int i0 = 0;
	while (i0 < OG_R) {
		filep >> passengers[i0][0] >> passengers[i0][1]; // extracts 2 floating point values seperated by whitespace
		i0++;
	}

	double** mandatory = new double* [N];//mandatory Stations
	for (i = 0; i < N; i++) {
		mandatory[i] = new double[2];
	}
	ifstream filem("data/input/" + igen + "mandatory.txt");
	i0 = 0;
	while (i0 < N) {
		filem >> mandatory[i0][0] >> mandatory[i0][1]; // extracts 2 floating point values seperated by whitespace
		i0++;
	}

	double** optional = new double* [(N - 1) * M]; // optinal stations
	for (i = 0; i < (N - 1) * M; i++) {
		optional[i] = new double[2];
	}
	ifstream fileo("data/input/" + igen + "optional" + to_string(M) + ".txt");
	i0 = 0;
	while (i0 < (N - 1) * M) {
		fileo >> optional[i0][0] >> optional[i0][1]; // extracts 2 floating point values seperated by whitespace
		i0++;
	}
	default_random_engine generator0;
	double timestamps[OG_R];
	ifstream filetp("data/input/" + igen + "timestamps" + to_string(OG_R) + ".txt");

	// Arrival times of the passengers 
	double OG_arrivals[OG_R1];
	double arrivals[R1];
	ifstream filea("data/input/" + igen + "arrivals" + to_string(OG_R) + ".txt");
	inst << endl << "Desired arrival times (DAT) of the passengers and time-stamp of request (TSR) in seconds: " << endl;
	i0 = 0;
	//cout << "SO2: " << SO2 << endl;
	double minTS = INT16_MAX;
	while (i0 < R1) {
		filetp >> timestamps[i0];
		if (minTS > timestamps[i0]) minTS = timestamps[i0];
		if (R != 3) timestamps[i0] = -1;
		filea >> OG_arrivals[i0];
		arrivals[i0] = OG_arrivals[i0];
		if (R != 3) inst << "p_" << i0 + 1 << ":\tDAT=" << OG_arrivals[i0] << "\tTSR before the start of operation" << endl;
		else inst << "p_" << i0 + 1 << ":\tDAT=" << OG_arrivals[i0] << "\tTSR=" << int(timestamps[i0]) << endl;
		i0++;
	}
	//cout << i0 << endl;
	while (i0 < OG_R1) {
		filetp >> timestamps[i0];
		if (minTS > timestamps[i0]) minTS = timestamps[i0];
		filea >> OG_arrivals[i0];
		inst << "p_" << i0 + 1 << ":\tDAT=" << OG_arrivals[i0] << "\tTSR=" << int(timestamps[i0]) << endl;
		i0++;
	}
	// Departure times of the passengers 
	double departures[R2];
	double OG_departures[OG_R2];
	ifstream filed("data/input/" + igen + "departures" + to_string(OG_R) + ".txt");
	inst << endl << "Desired departure times (DDT) of the passengers and time-stamp of request (TSR) in seconds: " << endl;
	i0 = 0;
	while (i0 < R2) {
		filetp >> timestamps[i0 + OG_R1];
		if (R != 3) timestamps[i0 + OG_R1] = -1;
		filed >> OG_departures[i0];
		departures[i0] = OG_departures[i0];
		if (R != 3) inst << "p_" << i0 + 1 + OG_R1 << ":\tDDT=" << OG_departures[i0] << "\tTSR before the start of operation" << endl;
		else inst << "p_" << i0 + 1 + OG_R1 << ":\tDDT=" << OG_departures[i0] << "\tTSR=" << int(timestamps[i0 + OG_R1]) << endl;
		i0++;
	}
	while (i0 < OG_R2) {
		filetp >> timestamps[i0 + OG_R1];
		filed >> OG_departures[i0];
		inst << "p_" << i0 + 1 + OG_R1 << ":\tDDT=" << OG_departures[i0] << "\tTSR=" << int(timestamps[i0 + OG_R1]) << endl;
		i0++;
	}

	//calculate travel time using manhattan distance
	double** traveltimep = new double* [OG_R]; // travel times of people between passangers and stations
	for (i = 0; i < OG_R; i++) {
		traveltimep[i] = new double[S];
		for (j = 0; j < S; j++) {
			traveltimep[i][j] = INT32_MAX;
		}
	}
	double** traveltimes = new double* [S];
	for (i = 0; i < S; i++) {
		traveltimes[i] = new double[S];
		for (j = 0; j < S; j++) {
			traveltimes[i][j] = INT32_MAX;
		}
	}// travel times of buses between stations

	if (!inst_gen) {
		for (i = 0; i < OG_R; i++) {
			for (j = 0; j < N; j++) {
				traveltimep[i][j] = (abs(passengers[i][0] - mandatory[j][0]) + abs(passengers[i][1] - mandatory[j][1])) * 1000 / pspeed;
			}

			for (j = N; j < S; j++) {
				traveltimep[i][j] = (abs(passengers[i][0] - optional[j - N][0]) + abs(passengers[i][1] - optional[j - N][1])) * 1000 / pspeed;
			}
		}
	}
	else {
		read_walking_matrix("data/input//walking_data.csv", traveltimep, OG_R, S);
	}


	inst << endl << "Walking time between passengers and bus stops in seconds: " << "\npassengers correspond with the rows, bus stops correspond with the columns \nthe mandatory stops are listed first, then the optional stops are listed" << endl;
	for (i = 0; i < OG_R; i++) {
		for (j = 0; j < S; j++) {
			if (!inst_gen) inst << int(traveltimep[i][j]) << "\t";
			else if (traveltimep[i][j] != INT32_MAX) inst << int(traveltimep[i][j]) << "\t";
			else inst << "x\t";
			//walk_p << int(traveltimep[i][j]) << "\t";
		}
		inst << endl;
		//walk_p << endl;
	}
	//walk_p.close();
	//exit(0);
	if (!inst_gen) {
		for (i = 0; i < N; i++) {
			for (j = 0; j < N; j++) {
				traveltimes[i][j] = (abs(mandatory[i][0] - mandatory[j][0]) + abs(mandatory[i][1] - mandatory[j][1])) * 1000 / bspeed;
			}
			for (j = N; j < S; j++) {
				traveltimes[i][j] = (abs(mandatory[i][0] - optional[j - N][0]) + abs(mandatory[i][1] - optional[j - N][1])) * 1000 / bspeed;
			}
		}

		for (i = N; i < S; i++) {
			for (j = 0; j < N; j++) {
				traveltimes[i][j] = (abs(optional[i - N][0] - mandatory[j][0]) + abs(optional[i - N][1] - mandatory[j][1])) * 1000 / bspeed;
			}
			for (j = N; j < S; j++) {
				traveltimes[i][j] = (abs(optional[i - N][0] - optional[j - N][0]) + abs(optional[i - N][1] - optional[j - N][1])) * 1000 / bspeed;
			}
		}
	}
	else {
		cout << "check\n";
		read_travel_matrix("data/input/travel_time.csv", traveltimes, S);
		cout << "check\n";
	}

	inst << endl << "Travel time between bus stops in seconds: " << endl;
	for (i = 0; i < S; i++) {
		for (j = 0; j < S; j++) {
			inst << int(traveltimes[i][j]) << "\t";
		}
		inst << endl;
	}
	inst.close();


	//exit(0);

	//************************************************* PRE PROCESSING ********************************************************
	
	double* tempdist = new double[S];
	int* index = new int[S];
	int** closestS = new int* [S];
	for (i = 0; i < S; i++) {
		closestS[i] = new int[S];
	}
	//std::cout << "Stops: \n";
	for (i = 0; i < S; i++) {
		//initialize and copy distance of cities to temp array
		for (j = 0; j < S; j++) {
			if (j != i) tempdist[j] = traveltimes[i][j];
			else tempdist[j] = 100000;
			index[j] = j;
		}

		//sort according to dist
		quickSort(index, tempdist, 0, S - 1);

		//keep track of best neighbors 
		for (l = 0; l < S; l++) {
			closestS[i][l] = index[l]; //index
			//std::cout << index[l] << " ";
		}
		//std::cout << " \n";
	}

	int** closestPS = new int* [OG_R];
	for (i = 0; i < OG_R; i++) {
		closestPS[i] = new int[S];
	}
	//std::cout << "Passengers: \n";
	for (p = 0; p < OG_R; p++) {
		//initialize and copy distance of cities to temp array
		for (j = 0; j < S; j++) {
			tempdist[j] = traveltimep[p][j];
			index[j] = j;
		}
		//sort according to dist
		quickSort(index, tempdist, 0, S - 1);

		//keep track of best neighbors 
		for (j = 0; j < S; j++) {
			closestPS[p][j] = index[j];
			//std::cout << index[j] << " ";
		}
	}

	//remove memory
	delete[] index;
	delete[] tempdist;

	//generator0.seed(400005550007);
	//uniform_int_distribution<int> rRequests(0, R - 1);
	//uniform_int_distribution<int> rStations(0, S - 1);
	///uniform_int_distribution<int> oneto100(1, 100);
	uniform_int_distribution<int> NEXT(0, M);
	// ---------------- Shortest route
	double short_route = 0;
	for (i = 0; i < N - 1; i++) {
		short_route += traveltimes[i][i + 1];
	}
	//------- Determine best route
	int best_route[S];
	//*

	best_route[0] = 0;
	std::cout << best_route[0] << " ";

	int ma = 0;
	int oc = 0;
	int nopt = 100;
	for (i = 1; i < S; i++) {
		if (oc < M) {
			best_route[i] = N + ma * M + oc;
			oc++;
		}
		else {
			oc = 0;
			ma++;
			best_route[i] = ma;
		}
		//std::cout << best_route[i] << " ";
	}
	//std::cout << endl;

	int start, end, nextindex, cc, mid, temp0;
	double currentcost, ncost, totE = 0, dE0;
	for (i = 0; i < N - 1; i++) {
		std::uniform_int_distribution<int> NEXT(i * (M + 1), (M + 1) * i + M + 1); //look only in a cluster (mandatory stops included)
		for (t = 0; t < nopt; t++) {//number of opt operations
			j = NEXT(generator0);
			while (j == (M + 1) * i + M + 1) {
				j = NEXT(generator0);
			}
			l = NEXT(generator0);
			// two nodes cannot be the same of both be the mandatory stops at once
			while (l == j || l == (M + 1) * i + M + 1) {
				l = NEXT(generator0);
			}

			if (j > l) {
				start = l;
				end = j;
			}
			else {
				start = j;
				end = l;
			}
			nextindex = end + 1;
			currentcost = traveltimes[best_route[start]][best_route[start + 1]] + traveltimes[best_route[end]][best_route[nextindex]];
			ncost = traveltimes[best_route[start]][best_route[end]] + traveltimes[best_route[start + 1]][best_route[nextindex]];
			//std::cout << " Current cost: " << currentcost << " New Cost: " << newcost << " \n";
			//--------------------------------------------------------------------SA --------------------------------------------------------------------------
			dE0 = (ncost - currentcost);
			if (dE0 < 0) {
				//currentcost = ncost;
				totE -= -dE0;
				mid = (end - start) / 2;
				//2opt
				for (cc = 1; cc <= mid; cc++) {
					temp0 = best_route[start + cc];
					best_route[start + cc] = best_route[end + 1 - cc];
					best_route[end + 1 - cc] = temp0;
				}
			}
		}
	}

	currentcost = 0;
	for (i = 0; i < S; i++) {
		std::cout << best_route[i] << " ";
		if (i != S - 1) {
			currentcost += traveltimes[i][i + 1];
		}
	}
	cout << "\nTime big route: " << round(currentcost / 60 * 100) / 100 << " min" << endl;
	cout << "Time short route: " << round(short_route / 60 * 100) / 100 << " min" << endl;

	for (p = 0; p < OG_R; p++) {
		if (p < OG_R1) cout << "p_" << p << "\tDAT=" << round(OG_arrivals[p] / 60) << "\tclosest stop: " << closestPS[p][0] << endl;
		else cout << "p_" << p << "\tDDT=" << round(OG_departures[p - OG_R1] / 60) << "\tclosest stop: " << closestPS[p][0] << endl;
	}
	//----------------------------------------------------------------------------- INITIALIZE ----------------------------------------------------------------------------------------------------

	//list of lists: each element in higher list corresponds with a passenger, each passenger gets a list of length three
	//this list assigns passenger as follows: p_i= [bus, trip, bus stop]
	//int** ysol = new int* [R];
	int** b_ysol = new int* [OG_R];
	int** ysol = new int* [OG_R];
	for (i = 0; i < OG_R; i++) {
		//ysol[i] = new int[3];
		b_ysol[i] = new int[3];
		ysol[i] = new int[3];

		b_ysol[i][0] = -1;
		b_ysol[i][1] = -1;
		b_ysol[i][2] = -1;

		ysol[i][0] = -1;
		ysol[i][1] = -1;
		ysol[i][2] = -1;
	}
	//routing: [bus, trip, bus stops]
	//vector<vector<vector<int>>> xsol(B);
	vector<vector<vector<int>>> b_xsol(B);
	vector<vector<vector<int>>> xsol(B);

	//D: [bus, trip, departure time]
	//vector<vector<vector<double>>> Dsol(B);
	vector<vector<vector<double>>> b_Dsol(B);
	vector<vector<vector<double>>> Dsol(B);
	double currtime = minTS;
	double endtime = minTS + TS;
	double tt;
	int X1, X2;

	double bd[B];
	int trips[B];
	for (b = 0; b < B; b++) {
		bd[b] = minTS - 50 * 60;
		trips[b] = 0;
	}
	double freqN[N];
	for (b = 0; b < N; b++) {
		freqN[b] = -1;
	}
	vector<int>  route;
	vector<double> timetable;
	if (R == 3) {
		while (currtime < endtime) {

			//determine next bus 
			b = iMin(bd, B);
			//cout << "\n   +++++++++++++ BUS " << b << " on trip " << trips[b] << " bd: " << round(bd[b] / 60) << endl;

			//initialize timetable and route 
			route.clear();
			timetable.clear();
			timetable.push_back(max(freqN[0] + max(OGxt / 2, 10*60), bd[b]));
			route.push_back(0);
			tt = -1;
			for (i = 1; i < N; i++) {
				route.push_back(i);
				timetable.push_back(timetable[i - 1] + traveltimes[i - 1][i]);
				if (freqN[i] != -1 && timetable[i] - freqN[i] > tt)tt = timetable[i] - freqN[i];
			}

			if (tt > OGxt) {
				for (i = 0; i < N; i++) {
					timetable[i] -= (tt - OGxt);
				}
			}

			if (timetable[0] < bd[b]) {
				cout << " INFEASIBLE for xt\n";
				break;
			}
			b_xsol[b].push_back(route);
			b_Dsol[b].push_back(timetable);
			//intialize capacities

			//update freqN
			X1 = b_xsol[b][trips[b]].size();
			for (i = 0; i < X1; i++) {
				if (b_xsol[b][trips[b]][i] < N && (freqN[b_xsol[b][trips[b]][i]] < b_Dsol[b][trips[b]][i] || freqN[b_xsol[b][trips[b]][i]] - b_Dsol[b][trips[b]][i] > OGxt)) {
					freqN[b_xsol[b][trips[b]][i]] = b_Dsol[b][trips[b]][i];
					//t++;
				}
			}

			currtime = b_Dsol[b][trips[b]].back();
			bd[b] = currtime + short_route;
			trips[b]++;
		}

		for (p = 0; p < OG_R; p++) {
			b_ysol[p][0] = -1;
			b_ysol[p][1] = -1;
			b_ysol[p][2] = -1;
		}
	}
	//------------------------------------------------------------- Make INITIAL planning with requests that are already recieved ------------------------------------------------------------------
	else{
		if (SOpt) {
		double** ttraveltimep = new double* [R]; // travel times of people between passangers and stations
		for (i = 0; i < R; i++) {
			ttraveltimep[i] = new double[S];
		}
		for (i = 0; i < R; i++) {
			if (i < R1) {
				for (j = 0; j < N; j++) {
					ttraveltimep[i][j] = (abs(passengers[i][0] - mandatory[j][0]) + abs(passengers[i][1] - mandatory[j][1])) * 1000 / pspeed;
				}

				for (j = N; j < S; j++) {
					ttraveltimep[i][j] = (abs(passengers[i][0] - optional[j - N][0]) + abs(passengers[i][1] - optional[j - N][1])) * 1000 / pspeed;
				}
			}
			else {
				for (j = 0; j < N; j++) {
					ttraveltimep[i][j] = (abs(passengers[i + OG_R1 - R1][0] - mandatory[j][0]) + abs(passengers[i + OG_R1 - R1][1] - mandatory[j][1])) * 1000 / pspeed;
				}

				for (j = N; j < S; j++) {
					ttraveltimep[i][j] = (abs(passengers[i + OG_R1 - R1][0] - optional[j - N][0]) + abs(passengers[i + OG_R1 - R1][1] - optional[j - N][1])) * 1000 / pspeed;
				}
			}
		}
		double* ttempdist = new double[S];
		int* tindex = new int[S];
		int** tclosestPS = new int* [R];
		for (i = 0; i < R; i++) {
			tclosestPS[i] = new int[S];
		}
		//std::cout << "Passengers: \n";
		for (p = 0; p < R; p++) {
			//initialize and copy distance of cities to temp array
			for (j = 0; j < S; j++) {
				ttempdist[j] = ttraveltimep[p][j];
				tindex[j] = j;
			}
			//sort according to dist
			quickSort(tindex, ttempdist, 0, S - 1);

			//keep track of best neighbors 
			for (j = 0; j < S; j++) {
				tclosestPS[p][j] = tindex[j];
				//std::cout << index[j] << " ";
			}
		}

		//remove memory
		delete[] tindex;
		delete[] ttempdist;

		uniform_real_distribution<float> r01(0, 1);
		double u_cost = INT32_MAX, u_RT;

		vector<float> lPM1;
		vector<float> lPM2;
		vector<float> lPM3;
		vector<float> lFPM;
		vector<float> lXT;
		vector<float> lC;

		vector<float> b_lPM1;
		vector<float> b_lPM2;
		vector<float> b_lPM3;
		vector<float> b_lFPM;
		vector<float> b_lXT;
		vector<float> b_lC;

		vector < vector<float>> lPM1b;
		vector < vector<float>> lPM2b;
		vector < vector<float>> lPM3b;
		vector < vector<float>> lFPMb;
		vector < vector<float>> lXTb;
		vector < vector<float>> lCb;

		vector<double> Costb;
		double best_cost = INT32_MAX;
		double tstart_time = clock();
		// +++++++++++++++++++++++++   LNS params +++++++++++++++++++++++++++++
		double Ts = 1000, T_end = 0.001;
		//double alph = 0.985;
		const double nhp = 0.125;
		//int LL = 30;
		const int des = 15;
		//double Tmax = Ts;
		const double lam = 10;
		int r_i = 0, stop_it = 150000;
		double bb_cost = INT32_MAX;
		random_device r;
		std::vector<std::default_random_engine> generators;
		normal_distribution<float> PM(0.6, 0.25);
		uniform_real_distribution<float> gXT(0.6, 1);
		uniform_real_distribution<float> gC(0.25, 1);
		uniform_real_distribution<float> PlanB1(0.95, 1);
		uniform_real_distribution<float> PlanB2(0, 0.05);
		uniform_real_distribution<float> PlanB3(0.35, 0.45);
		//int nN;
		for (int i = 0, nN = omp_get_max_threads(); i < nN; ++i) {
			generators.emplace_back(default_random_engine(r()));
		}
		std::atomic<bool> INFEASS0(false);
		//omp_set_num_threads(12);
		//int give = 0;
		//cout << omp_get_max_threads() << endl;
#pragma omp parallel firstprivate(traveltimes, ttraveltimep, arrivals, departures,tclosestPS,closestS, S,M,N,B,short_route, best_route,C,TS,OGxt,dw,d_dl,d_de,d_ae,d_al,c1,c2,c3)
		{
			//double countInfeas1 = 0, countInfeas2 = 0, countFeas = 0;
			default_random_engine& generator = generators[omp_get_thread_num()];
			//generator.seed((omp_get_thread_num()) * 800051150);
			//int iii, stop;
			int xt;
			double UBxt;
			//int LL = 30;

			int i, j, b, t, p, l, s;
			double pm1 = 1, pm2 = 1, pm3= 1, fpm=1;
			double b_next;
			vector<int> route;
			vector<float> dPM10;
			vector<float> dPM20;
			vector<float> dPM30;
			vector<float> dFPM0;
			vector<float> dXT0;
			vector<float> dC0;
			int temp;
			vector<double> IFC;
			int FCi = 0;
			//double dE;
			vector<double> timetable;
			vector<double> tempt;
			double timewindow, timewindow2;
			//double Arr;


			int trips[B]; //keeps track of which trip each bus is at needs to be updated
			int best_stop;
			double freqN[N];
			double newfreqN[N];

			double bd[B];
			int yk[R][3];
			double startopt;

			int countp, p1, p2, cp1, cp2;
			bool in, in2;
			double threshold, tt;

			double cost, b_cost;
			double l_arr, l_dep;

			int b_it = 0;

			double minF = INT16_MAX, min_al = INT16_MAX, min_ae = INT16_MAX, min_dl = INT16_MAX, min_de = INT16_MAX;
			int nS, dst;
			double diffa1, diffa2, diffd1, diffd2, diffF;
			double difBD, maxFW, maxRW;//max time to arrive or depart earlier
			//add addtional passenger with DAT
			//*
			double extra = 0, b_extra = 0;
			int indexpt[R1];
			int indexpt2[R2];
			double temparrivals[R1];
			double tempdept[R2];
			//routing: [bus, trip,stops]
			vector<vector<vector<int>>> xk(B);
			//D: [bus, trip,departure time]
			vector<vector<vector<double>>> Dk(B);
			bool INFEAS1 = false, INFEAS2 = false, INFEAS3 = false, INFEAS4 = false, INFEAS5 = false;


			//generator.seed(7851 * (omp_get_thread_num()));
			while (!INFEASS0) {
				//cout << omp_get_thread_num() << endl;
				//cout << "Start \n";
				for (s = 0; s < B; s++) {
					xk[s].clear();
					Dk[s].clear();
				}
				//if (!INFEASS0) {
				//cout << run << endl;
				//generator.seed(48441 * (omp_get_thread_num()+countInfeas1+countInfeas2));
				//cout << run << " parallel "<< endl;

				//std::cout << "\n************************************************************************** Run number: " << run + 1 << endl;
				//logres = "";
				for (p = 0; p < R; p++) {
					yk[p][0] = -1;
					yk[p][1] = -1;
					yk[p][2] = -1;

				}

				for (p = 0; p < R1; p++) {
					indexpt[p] = p;
					temparrivals[p] = arrivals[p];
				}

				for (p = 0; p < R2; p++) {
					indexpt2[p] = p;
					tempdept[p] = departures[p];
				}
				quickSort(indexpt, temparrivals, 0, R1 - 1);
				quickSort(indexpt2, tempdept, 0, R2 - 1);
				best_stop = 0;
				startopt = tempdept[0] - 1000;
				threshold = 0, tt = 0;
				cost = 0, b_cost = 0;

				route.clear();

				timetable.clear();
				tempt.clear();
				//cout << "check\n";
				dPM10.clear();
				dPM20.clear();
				dPM30.clear();
				dFPM0.clear();
				dXT0.clear();
				dC0.clear();
				l_arr = arrivals[indexpt[0]], l_dep = departures[indexpt2[0]] + short_route * 0.75;
				//logres += "Run: " + to_string(run + 1) + "\t";
				//start_time = clock();
				//************************************Initial Solution***************************************
				for (i = 0; i < N; i++) {
					freqN[i] = -INT16_MAX;
				}
				countp = p1 = p2 = 0;
				for (b = 0; b < B; b++) {
					trips[b] = 0;
					bd[b] = startopt;//start of optimization
				}
				timetable.push_back(0);

				p1 = p2 = countp = 0;
				b = 0;
				b_next = 0.0;
				//cout << " END of PH: " << int(TS + startopt) / 60 << endl;
				b_it = -1;

				while (timetable.back() < TS + startopt && p1 + p2 < R) {//until TS is over+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++	
					b_it++;

					pm1 = PM(generator);
					pm2 = PM(generator);
					pm3 = PM(generator);
					fpm = PM(generator);

					if (pm1 > 1)pm1 = 1;
					if (pm1 < 0)pm1 = 0;
					if (pm2 > 1)pm2 = 1;
					if (pm2 < 0)pm2 = 0;
					if (pm3 > 1)pm3 = 1;
					if (pm3 < 0)pm3 = 0;
					if (fpm > 1)fpm = 1;
					if (fpm < 0)fpm = 0;

					UBxt = gXT(generator);
					xt = int(UBxt * OGxt);

					dPM10.push_back(pm1);
					dPM20.push_back(pm2);
					dPM30.push_back(pm3);
					dFPM0.push_back(fpm);
					dXT0.push_back(UBxt);

					float Cc = gC(generator);
					dC0.push_back(Cc);
					C = max(5, int(Cc * C_OG));

					//xt = OGxt;
					//if (infeastemp > 100) fpm = 1;
					//cout << " xt: " << xt / 60 << endl;
					//determine which bus is available first
					b = iMin(bd, B);
					b_next = iMin2(bd, B);
					//cout << "++++++ || Bus: " << b << " bd: " << int(bd[b] / 60) << " trip: " << trips[b] << " || ++++++\n";
					//for (i = 0; i < B; i++) cout << int(bd[i] / 60) << " ";
					//cout << endl;
					//cout << "bd1: " << int(bd[b]/60) << " bd2: " << int(b_next/60) << endl;
					//Make route only with mandatory stops, ASAP
					route.clear();
					timetable.clear();
					tempt.clear();
					tt = bd[b];
					timetable.push_back(tt);
					route.push_back(0);
					for (i = 1; i < N; i++) {
						route.push_back(i);
						tt += traveltimes[i - 1][i];
						timetable.push_back(tt);
					}
					//Assignement: First look at R1
					timewindow = timewindow2 = INT32_MAX;
					cp1 = cp2 = 0;

					//++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++ CASE I ++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
					if (l_arr < l_dep && p1 < R1 || p2 == R2) {
						//cout << "R1" << endl;
						p = 0;
						while (cp1 + cp2 < C && countp < R && p < R1 && (int)(arrivals[indexpt[p]] - timewindow) <= d_ae + d_al) {
							//cout << " check start\n";
							//cout << cp1 + cp2 << "<" << C << "  " << countp << "<" << R << "  " << (int)(arrivals[indexpt[p]] - timewindow) << "< " << d_ae + d_al << endl;
							if (ttraveltimep[indexpt[p]][tclosestPS[indexpt[p]][0]] > dw) {
								std::cout << " Infeasible solution, for walking times " << endl;
								//cout << "stop: " << tclosestPS[indexpt[p]][0] << endl;
								exit(0);
							}
							if (temparrivals[indexpt[p]] == -1) {
								//cout << "passenger already in solution \n";
								//cout << "p: " << p << " indexpt: " << indexpt[p] << endl;
								p++;
								if (p > R1)break;
								continue;//if this is already in the solution, continue to next passenger
							}
							if (cp1 == 0)timewindow = arrivals[indexpt[p]];

							//Assign bus stop to passenger
							//choose best stop and update route
							if (cp1 == 0) {
								//cout << " check 2 \n";
								best_stop = bestStop(timetable, route, traveltimes, ttraveltimep, tclosestPS, indexpt[p], N,
									dw, best_route, M, S, bd[b], freqN, xt, best_stop, arrivals[indexpt[p]],
									arrivals[indexpt[p]], d_ae, d_al, yk, arrivals, R1, R2, b, trips[b], b_next, fpm);
								//cout << " check stop 0\n";
								if (best_stop == -1) {
									//cout << "passenger " << indexpt[p] << " --> Skip this one, not good for bus stop\n";
									p++;
									//cout << "Next passenger " << p1 << endl;
									if (p > R1)break;
									continue;
								}
								//cout << " --- FIRST passenger added : p= " << indexpt[p] << "\n";
								//Assign bus to passenger
								yk[indexpt[p]][0] = b;
								yk[indexpt[p]][1] = trips[b];
								yk[indexpt[p]][2] = best_stop;
								temparrivals[indexpt[p]] = -1;
								tempt.push_back(arrivals[indexpt[p]]);
								p++;
								p1++;
								cp1++;
								countp++;
								if (p > R1)break;
							}
							else {
								temp = bestStop(timetable, route, traveltimes, ttraveltimep, tclosestPS, indexpt[p], N, dw,
									best_route, M, S, bd[b], freqN, xt, best_stop, tempt.back(),
									arrivals[indexpt[p]], d_ae, d_al, yk, arrivals, R1, R2, b, trips[b], b_next, fpm);
								//cout << " check stop n: " << temp <<"\n";
								if (temp == -1) {
									//cout << "passenger " << indexpt[p] << " --> Skip this one, not good for bus stop\n";
									p++;//next stop of a passenger with later edt is before a previous stop with a passenger with earlier edt, not possible OR edts are too different
									if (p > R1)break;
									continue;
								}
								else {
									// time to go from prev to next stop on a full route 
									best_stop = temp;
									//Assign bus to passenger
									yk[indexpt[p]][0] = b;
									yk[indexpt[p]][1] = trips[b];
									yk[indexpt[p]][2] = best_stop;
									temparrivals[indexpt[p]] = -1;
									tempt.push_back(arrivals[indexpt[p]]);
									p++;
									p1++;
									cp1++;
									countp++;
									if (p > R1)break;
									//cout << "-- NEXT passenger added : p= " << indexpt[p] << "\n";
									//cout << " indexpt: " << indexpt[p] << " arrivals p: " << arrivals[indexpt[p]] <<  endl;
									//cout << " p: " << p << " indexpt: " << indexpt[p] << endl;
								}
							}
						}
						//cout << " check end\n";
						//cout << "timetable:" << timetable.size() << " route: " << route.size() << " Passengers added: " << tempt.size() << endl;

					}
					// //++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++ CASE II ++++++++++++++++++++++++++++++++++++++++++++ Now look at R2
					//*
					if ((l_arr >= l_dep && p2 < R2) || cp1 == 0) {
						//cout << "R2" << endl;
						//cout << timetable.size() << endl;
						timetable.clear();
						tt = bd[b];
						timetable.push_back(tt);
						for (i = 1; i < N; i++) {
							tt += traveltimes[i - 1][i];
							timetable.push_back(tt);
						}

						p = 0;
						while (cp2 < C && countp < R && p < R2) {
							//cout << p2 << "<" << C << "  " << countp << "<" << R << " " << p << "< " << R2 << endl;
							//cout << "passenger " << indexpt2[p] + R1 << "  ------------- \n";
							if (ttraveltimep[R1 + indexpt2[p]][tclosestPS[R1 + indexpt2[p]][0]] > dw) {
								std::cout << " Infeasible solution, for walking times " << endl;
								exit(0);
							}
							if (tempdept[indexpt2[p]] == -1) {
								//cout << "passenger already in solution \n";
								p++;
								continue;//if this is already in the solution, continue to next passenger
							}
							t = 0;
							int ttS = timetable.size();
							for (i = 0; i < ttS; i++) {
								if (route[i] < N) {
									newfreqN[t] = timetable[i];
									t++;
								}
							}
							if (cp2 == 0) {
								best_stop = bestStop2(timetable, route, traveltimes, ttraveltimep, tclosestPS, indexpt2[p] + R1, N,
									dw, best_route, M, S, bd[b], freqN, newfreqN, xt, best_stop, departures[indexpt2[p]],
									departures[indexpt2[p]], d_de, d_dl, yk, departures, R1, R2, b, trips[b], b_next, fpm);
								if (best_stop == -1) {
									//cout << "passenger " << indexpt2[p] + R1 << " --> Skip this one, not good for bus stop\n";
									p++;
									//cout << "Next passenger " << p1 << endl;
									continue;
								}
								//cout << " --- FIRST passenger added : p= " << indexpt2[p] + R1 << "\n";
								//Assign bus to passenger
								yk[R1 + indexpt2[p]][0] = b;
								yk[R1 + indexpt2[p]][1] = trips[b];
								//Assign bus stop to passenger
								//choose best stop and update route
								//cout << "  Best stop: " << best_stop << endl;
								//cout << "Passenger: " << R1 + indexpt2[p] << " edt: " << int(departures[indexpt2[p]] / 60) << " stop: " << best_stop << endl;
								tempt.push_back(departures[indexpt2[p]]);
								tempdept[indexpt2[p]] = -1;
								yk[R1 + indexpt2[p]][2] = best_stop;
								cp2++;
								p2++;
								countp++;
								p++;
							}
							else {
								temp = bestStop2(timetable, route, traveltimes, ttraveltimep, tclosestPS, indexpt2[p] + R1, N, dw,
									best_route, M, S, bd[b], freqN, newfreqN, xt, best_stop, tempt.back(),
									departures[indexpt2[p]], d_de, d_dl, yk, departures, R1, R2, b, trips[b], b_next, fpm);

								if (temp == -1) {
									//cout << "Passenger: " << R1 + indexpt2[p] << " edt: "<< int(departures[indexpt2[p]] / 60) << "Skip this one, not good for bus stop\n";
									p++;//next stop of a passenger with later edt is before a previous stop with a passenger with earlier edt, not possible OR edts are too different
									continue;
								}
								else {
									// time to go from prev to next stop on a full route 
									best_stop = temp;
									//Assign bus to passenger
									yk[R1 + indexpt2[p]][0] = b;
									yk[R1 + indexpt2[p]][1] = trips[b];
									//Assign bus stop to passenger
									yk[R1 + indexpt2[p]][2] = best_stop;
									//cout << "Passenger: " << R1 + indexpt2[p] << " edt: " << int(departures[indexpt2[p]] / 60) << " stop: " << best_stop << endl;
									tempt.push_back(departures[indexpt2[p]]);
									tempdept[indexpt2[p]] = -1;
									cp2++;
									p2++;
									countp++;
									p++;
									//cout << "-- NEXT passenger added : p= " << indexpt2[p] + R1 << "\n";
								}
							}
						}
						int temptS = tempt.size();
						if (p1 < R1 && l_arr >= l_dep && temptS == 0) {
							//++++++++++++++++++++++++++++++++ CASE I ++++++++++++++++++++++++++++++++++++++++++++
							//cout << "R1 again" << endl;
							p = 0;
							while (cp1 + cp2 < C && countp < R && p < R1 && (int)(arrivals[indexpt[p]] - timewindow) <= d_ae + d_al) {
								//cout << cp1 + cp2 << "<" << C << "  " << countp << "<" << R << "  " << (int)(arrivals[indexpt[p]] - timewindow) << "< " << d_ae + d_al << endl;
								if (ttraveltimep[indexpt[p]][tclosestPS[indexpt[p]][0]] > dw) {
									std::cout << " Infeasible solution, for walking times " << endl;
									exit(0);
								}
								if (temparrivals[indexpt[p]] == -1) {
									//cout << "passenger already in solution \n";
									//cout << "p: " << p << " indexpt: " << indexpt[p] << endl;
									p++;
									if (p > R1)break;
									continue;//if this is already in the solution, continue to next passenger
								}
								if (cp1 == 0)timewindow = arrivals[indexpt[p]];
								//Assign bus stop to passenger
								//choose best stop and update route
								if (cp1 == 0) {
									best_stop = bestStop(timetable, route, traveltimes, ttraveltimep, tclosestPS, indexpt[p], N,
										dw, best_route, M, S, bd[b], freqN, xt, best_stop, arrivals[indexpt[p]],
										arrivals[indexpt[p]], d_ae, d_al, yk, arrivals, R1, R2, b, trips[b], b_next, fpm);
									//cout << " check\n";
									if (best_stop == -1) {
										//cout << "passenger " << indexpt[p]  << " --> Skip this one, not good for bus stop\n";
										p++;
										//cout << "Next passenger " << p1 << endl;
										if (p > R1)break;
										continue;
									}
									//cout << " --- FIRST passenger added\n";
									//Assign bus to passenger
									yk[indexpt[p]][0] = b;
									yk[indexpt[p]][1] = trips[b];
									yk[indexpt[p]][2] = best_stop;
									temparrivals[indexpt[p]] = -1;
									tempt.push_back(arrivals[indexpt[p]]);
									p++;
									p1++;
									cp1++;
									countp++;
									if (p > R1)break;
									//cout << " --- FIRST passenger added : p= " << indexpt[p] << "\n";
								}
								else {
									temp = bestStop(timetable, route, traveltimes, ttraveltimep, tclosestPS, indexpt[p], N, dw,
										best_route, M, S, bd[b], freqN, xt, best_stop, tempt.back(),
										arrivals[indexpt[p]], d_ae, d_al, yk, arrivals, R1, R2, b, trips[b], b_next, fpm);
									//cout << " check\n";
									if (temp == -1) {
										//cout << "passenger " << indexpt[p] << " --> Skip this one, not good for bus stop\n";
										p++;//next stop of a passenger with later edt is before a previous stop with a passenger with earlier edt, not possible OR edts are too different
										if (p > R1)break;
										continue;

									}
									else {
										// time to go from prev to next stop on a full route 
										best_stop = temp;
										//Assign bus to passenger
										yk[indexpt[p]][0] = b;
										yk[indexpt[p]][1] = trips[b];
										yk[indexpt[p]][2] = best_stop;
										temparrivals[indexpt[p]] = -1;
										tempt.push_back(arrivals[indexpt[p]]);
										p++;
										p1++;
										cp1++;
										countp++;
										if (p > R1)break;
										//cout << "-- NEXT passenger added : p= " << indexpt[p] << "\n";
									}
								}

							}

						}
						//cout << "timetable:" << timetable.size() << " route: " << route.size() << " Passengers added: " << tempt.size() << endl;
					}
					//*/
					int temptS = tempt.size();
					if (temptS == 0) {
						timetable.clear();
						timetable.push_back(max(freqN[0] + xt, bd[b]));
						//timetable.push_back(freqN[0] + xt);
						tt = -1;
						for (i = 1; i < N; i++) {
							timetable.push_back(timetable[i - 1] + traveltimes[i - 1][i]);
							if (timetable[i] - freqN[i] > tt)tt = timetable[i] - freqN[i];
						}
						if (tt > xt) {
							for (i = 0; i < N; i++) {
								timetable[i] -= (tt - xt);
							}
						}
					}
					else {
						//add addtional passenger with DAT

						minF = INT16_MAX, min_al = INT16_MAX, min_ae = INT16_MAX, min_dl = INT16_MAX, min_de = INT16_MAX;
						nS = timetable.size() - 1;

						for (p = 0; p < R1; p++) {
							if (yk[p][0] == b && yk[p][1] == trips[b]) {
								//cout << "passenger " << p  << ": dat=" << int(arrivals[p] / 60) << " arr=" << int(timetable[nS] / 60) << endl;
								diffa1 = d_ae - (arrivals[p] - timetable[nS]);
								diffa2 = d_al - (timetable[nS] - arrivals[p]);
								if (arrivals[p] >= timetable[nS] && min_ae > diffa1) {
									min_ae = diffa1;
								}
								if (arrivals[p] <= timetable[nS] && min_al > diffa2) {
									min_al = diffa2;
								}
							}
						}

						dst = -1;
						int routeS = route.size(), timetableS = timetable.size();
						for (p = 0; p < R2; p++) {
							dst = -1;
							for (i = 0; i < routeS; i++) {
								if (yk[p + R1][0] == b && yk[p + R1][1] == trips[b] && yk[p + R1][2] == route[i]) {
									dst = i;
									break;
								}
							}
							if (dst != -1) {
								//cout << "passenger " << p +R1 << ": edt=" << int(departures[p] / 60) << " dept=" << int(timetable[dst] / 60) << " at stop: " <<route[dst] << endl;
								diffd1 = d_de - (departures[p] - timetable[dst]);
								diffd2 = d_dl - (timetable[dst] - departures[p]);
								if (departures[p] >= timetable[dst] && min_de > diffd1) {
									min_de = diffd1;
								}
								if (departures[p] <= timetable[dst] && min_dl > diffd2) {
									min_dl = diffd2;
								}
							}
						}
						if (freqN[0] != -INT16_MAX) {
							for (i = 0; i < timetableS; i++) {
								if (route[i] < N && freqN[route[i]] < timetable[i]) {
									diffF = xt - (timetable[i] - freqN[route[i]]);
									if (minF > diffF) minF = diffF;
								}
							}
						}

						//cout << " min diff al: " << int(min_al/60) << " min diff dl: " << int(min_dl/60) << " min diff Freq: " << int(minF / 60) << endl;
						//cout << " min diff ae: " << int(min_ae/60) << " min diff de: " << int(min_de/60) << endl;
						difBD = timetable[0] - bd[b];
						maxFW = min(min(min_al, min_dl), minF) * pm1; // max time to depart later 
						maxRW = min(min(min_ae, min_de), difBD) * pm2;//max time to arrive or depart earlier
						//add addtional passenger with DAT
						//*
						extra = 0;
						//cout << "Max time FW: " << int(maxFW/60) << " max time RW: " << int(maxRW/60) << "\nStart R1\n";
						//cout << "R1: " << cp1 << " R2: " << cp2 << endl;

						for (p = 0; p < R1; p++) {
							if (temparrivals[p] != -1) {
								//cout << "for p_" << p << " dat: "<<arrivals[p]/60<< " arrval: " << timetable.back()/60<<endl;
								i = 0;
								b_cost = INT32_MAX;
								s = -1;
								if (arrivals[p] >= timetable.back()) {
									if (arrivals[p] - timetable.back() <= d_ae) extra = 0;
									else extra = (arrivals[p] - timetable.back()) - d_ae;
								}
								else {
									if (timetable.back() - arrivals[p] <= d_al) extra = 0;
									else extra = d_al - (timetable.back() - arrivals[p]);
								}
								while (ttraveltimep[p][tclosestPS[p][i]] < dw * pm3) {
									//cout << "try stop: " << tclosestPS[p][i] << endl;
									routeS = route.size() - 1;
									for (t = 0; t < routeS; t++) {
										in = false;
										if (tclosestPS[p][i] == route[t]) {
											in = true;
											break;
										}
									}
									in2 = false;
									if (in && arrivals[p] - timetable.back() - maxFW <= d_ae && timetable.back() - maxRW - arrivals[p] <= d_al) {
										in2 = true;
									}

									if (in2 && cp1 + cp2 < C) {
										//cout << " ++++++++++++ passenger " << p << " (dat=" << int(arrivals[p] / 60) << ") stop " << tclosestPS[p][i] << " (tt=" << int(timetable.back() / 60) << ") possible extra: " << int(extra / 60) << endl;
										cost = c2 * ttraveltimep[p][tclosestPS[p][i]] + c3 * abs(timetable.back() - arrivals[p]) + c3 * (cp1 + cp2) * (abs(extra));
										if (b_cost > cost) {
											b_cost = cost;
											s = route[t];
										}
									}
									i++;
								}
								if (b_cost != INT32_MAX) {
									//cout << "Added DAT \t";
									//cout << "Passenger: " << p << " arr: " << arrivals[p] / 60 << endl;
									yk[p][0] = b;
									yk[p][1] = trips[b];
									yk[p][2] = s;
									//cout << "Passenger: " << R1 + p << endl;
									temparrivals[p] = -1;//indicate this passenger is onboard already
									p1++;
									cp1++;
									countp++;
									//break;

									//update timetable
									//cout << " move tt with " << int(extra / 60) << endl;
									timetableS = timetable.size();
									for (l = 0; l < timetableS; l++) {
										timetable[l] += extra;
									}
									//update intermediary parameters
									minF = INT16_MAX, min_al = INT16_MAX, min_ae = INT16_MAX, min_dl = INT16_MAX, min_de = INT16_MAX;
									nS = timetable.size() - 1;
									for (int p11 = 0; p11 < R1; p11++) {
										if (yk[p11][0] == b && yk[p11][1] == trips[b]) {
											diffa1 = d_ae - (arrivals[p11] - timetable[nS]);
											diffa2 = d_al - (timetable[nS] - arrivals[p11]);
											if (arrivals[p11] >= timetable[nS] && min_ae > diffa1) {
												min_ae = diffa1;
											}
											if (arrivals[p11] <= timetable[nS] && min_al > diffa2) {
												min_al = diffa2;
											}
										}
									}
									for (int p22 = 0; p22 < R2; p22++) {
										dst = -1;
										routeS = route.size();
										for (l = 0; l < routeS; l++) {
											if (yk[p22 + R1][0] == b && yk[p22 + R1][1] == trips[b] && yk[p22 + R1][2] == route[l]) {
												dst = l;
												break;
											}
										}
										if (dst != -1) {
											diffd1 = d_de - (departures[p22] - timetable[dst]);
											diffd2 = d_dl - (timetable[dst] - departures[p22]);
											if (departures[p22] >= timetable[dst] && min_de > diffd1) {
												min_de = diffd1;
											}
											if (departures[p22] <= timetable[dst] && min_dl > diffd2) {
												min_dl = diffd2;
											}
										}
									}
									if (freqN[0] != -INT16_MAX) {
										timetableS = timetable.size();
										for (l = 0; l < timetableS; l++) {
											if (route[l] < N && freqN[route[l]] < timetable[l]) {
												diffF = xt - (timetable[l] - freqN[route[l]]);
												if (minF > diffF) minF = diffF;
											}
										}
									}
									//cout << " min diff al: " << int(min_al/60) << " min diff dl: " << int(min_dl/60) << endl;
									//cout << " min diff ae: " << int(min_ae/60) << " min diff de: " << int(min_de/60) << " min diff Freq: " << int(minF/60) << endl;
									difBD = timetable[0] - bd[b];
									maxFW = min(min(min_al, min_dl), minF) * pm1; // max time to depart later 
									maxRW = min(min(min_ae, min_de), difBD) * pm2;//max time to arrive or depart earlier
									//cout << "  Added passenger " << p << " --> NEW Max time FW: " << int(maxFW/60) << " max time RW: " << int(maxRW/60) << endl;

								}
							}
						}

						//add addtional passenger with EDT
						///*
						//cout << "Start R2\nNEW Max time FW : " << int(maxFW / 60) << " max time RW : " << int(maxRW / 60) << endl;
						b_extra = 0;
						for (p = 0; p < R2; p++) {
							if (tempdept[p] != -1) {
								//cout << "try p_" << p+R1 << " dept: "<<departures[p]/60<<endl;
								i = 0;
								b_cost = INT32_MAX;
								s = -1;
								while (ttraveltimep[R1 + p][tclosestPS[p + R1][i]] < dw * pm3) {
									routeS = route.size() - 1;
									//if (R1 + p == 36)cout << tclosestPS[p + R1][i] << endl;
									for (t = 0; t < routeS; t++) {
										in = false;
										if (tclosestPS[p + R1][i] == route[t]) {
											//cout << "bus stop " << tclosestPS[p + R1][i] << " position " << t << endl;
											in = true;
											break;
										}
									}
									in2 = false;
									if (in && departures[p] - timetable[t] - maxFW <= d_de && timetable[t] - maxRW - departures[p] <= d_dl) {
										in2 = true;
									}
									//if (in2) cout << "bus stop " << tclosestPS[p + R1][i] << " position " << t << " length of tt: " << timetable.size() << endl;

									if (in2 && cp1 + cp2 < C) {
										//cout << "bus stop " << tclosestPS[p + R1][i] << " position " << t << " length of tt: " << timetable.size() << endl;
										if (departures[p] >= timetable[t]) {
											if (departures[p] - timetable[t] <= d_de)extra = 0;
											else extra = (departures[p] - timetable[t]) - d_de;
										}
										else {
											if (timetable[t] - departures[p] <= d_dl)extra = 0;
											else extra = d_dl - (timetable[t] - departures[p]);
										}
										//cout << "walking time " << traveltimep[R1 + p][tclosestPS[p + R1][i]] / 60 << endl;
										//cout << " ++++++++++++ passenger " << p+R1 << " (edt="<< int(departures[p]/60) << ") stop " << tclosestPS[p + R1][i] <<" (tt="<< int(timetable[t]/60) << ") possible extra: " << int(extra / 60)<< endl;
										cost = c2 * ttraveltimep[R1 + p][tclosestPS[p + R1][i]] + c3 * abs(timetable[t] - departures[p]) + c3 * (cp1 + cp2) * (abs(extra));
										if (b_cost > cost) {
											b_cost = cost;
											s = route[t];
											b_extra = extra;
										}
									}
									i++;
								}
								if (b_cost != INT32_MAX) {
									//cout << "Added EDT \t";
									//cout << "Passenger: " << R1 + p << " dept: " << departures[p] / 60 << endl;
									yk[p + R1][0] = b;
									yk[p + R1][1] = trips[b];
									yk[p + R1][2] = s;
									//cout << "Passenger: " << R1 + p << endl;
									tempdept[p] = -1;//indicate this passenger is onboard already
									p2++;
									cp2++;
									countp++;
									//break;
									//update timetable
									//cout << " move tt with " << int(b_extra / 60) << endl;
									timetableS = timetable.size();
									for (l = 0; l < timetableS; l++) {
										timetable[l] += b_extra;
									}
									//update intermediary parameters
									minF = INT16_MAX, min_al = INT16_MAX, min_ae = INT16_MAX, min_dl = INT16_MAX, min_de = INT16_MAX;
									nS = timetable.size() - 1;
									for (int p11 = 0; p11 < R1; p11++) {
										if (yk[p11][0] == b && yk[p11][1] == trips[b]) {
											diffa1 = d_ae - (arrivals[p11] - timetable[nS]);
											diffa2 = d_al - (timetable[nS] - arrivals[p11]);
											if (arrivals[p11] >= timetable[nS] && min_ae > diffa1) {
												min_ae = diffa1;
											}
											if (arrivals[p11] <= timetable[nS] && min_al > diffa2) {
												min_al = diffa2;
											}
										}
									}
									for (int p22 = 0; p22 < R2; p22++) {
										dst = -1;
										routeS = route.size();
										for (l = 0; l < routeS; l++) {
											if (yk[p22 + R1][0] == b && yk[p22 + R1][1] == trips[b] && yk[p22 + R1][2] == route[l]) {
												dst = l;
												break;
											}
										}
										if (dst != -1) {
											diffd1 = d_de - (departures[p22] - timetable[dst]);
											diffd2 = d_dl - (timetable[dst] - departures[p22]);
											if (departures[p22] >= timetable[dst] && min_de > diffd1) {
												min_de = diffd1;
											}
											if (departures[p22] <= timetable[dst] && min_dl > diffd2) {
												min_dl = diffd2;
											}
										}
									}
									if (freqN[0] != -INT16_MAX) {
										timetableS = timetable.size();
										for (l = 0; l < timetableS; l++) {
											if (route[l] < N && freqN[route[l]] < timetable[l]) {
												diffF = xt - (timetable[l] - freqN[route[l]]);
												if (minF > diffF) minF = diffF;
											}
										}
									}

									difBD = timetable[0] - bd[b];
									maxFW = min(min(min_al, min_dl), minF) * pm1; // max time to depart later 
									maxRW = min(min(min_ae, min_de), difBD) * pm2;//max time to arrive or depart earlier
									//cout << "    min diff al: " << int(min_al/60) << " min diff dl: " << int(min_dl/60) << endl;
									//cout << "    min diff ae: " << int(min_ae/60) << " min diff de: " << int(min_de/60) << " min diff Freq: " << int(minF/60) << endl;
									//cout << "  Added passenger " << p + R1 << " --> NEW Max time FW: " << int(maxFW / 60) << " max time RW: " << int(maxRW / 60) << endl;
								}
							}
						}
						//cout << "AFTER --> R1: " << cp1 << " R2: " << cp2 << endl;
						//*/

					}
					//cout << "R1+R2 done  Nroute: " << route.size() << " N_timetable: " << timetable.size() << endl;
					//update trip of bus b
					trips[b]++;
					//update bd of bus b
					bd[b] = timetable.back() + short_route;
					//update varaibles
					xk[b].push_back(route);
					Dk[b].push_back(timetable);
					//update frequency at mandatory 
					//cout << "xt constraints: \n";
					//cout << " ******* TIMETABLE: ";
					int timetableS = timetable.size();
					for (i = 0; i < timetableS; i++) {
						if (route[i] < N && (freqN[route[i]] < timetable[i] || freqN[route[i]] - timetable[i] > xt)) {
							//if(timetable[i] - freqN[route[i]]+1>xt) cout << int((timetable[i]- freqN[route[i]])) / 60 << " (s: "<< route[i]<< ") ";
							freqN[route[i]] = timetable[i];
							t++;
						}
						//cout << int(timetable[i] / 60) << " ";
					}
					//cout << endl << " FreqN: " ;
					//for (i = 0; i < N; i++) {
						//cout << int(freqN[i] / 60) << "  ";
					//}
					//cout << endl;
					/*
					cout << " timetable: \n";
					for (i = 0; i < timetable.size(); i++) {
						cout << int(timetable[i] / 60) << " ";
					}
					cout << endl;
					//*/
					//cout << "Passengers added to bus: " << cp1 + cp2 << endl;
					//cout << endl << "Number of passenger assigned: R2: " << p2 << " R1: " << p1 << endl << endl;
					if (p2 < R2) {
						for (p = 0; p < R2; p++) {
							if (tempdept[indexpt2[p]] != -1) {
								l_dep = departures[indexpt2[p]] + 0.75 * short_route;
								break;
							}
						}
					}
					if (p1 < R1) {
						for (p = 0; p < R1; p++) {
							if (temparrivals[indexpt[p]] != -1) {
								l_arr = arrivals[indexpt[p]];
								break;
							}
						}
					}
				}

				//elapsed_time = (double)(clock() - start_time) / CLK_TCK;
				//iii++;
				INFEAS1 = false, INFEAS2 = false, INFEAS3 = false, INFEAS4 = false, INFEAS5 = false;
				//cost = INT32_MAX;
				//cout <<  " thread: " + to_string(omp_get_thread_num()) +  "\n";
				if (countp != R) {
					continue;
				}
				else {
					//if(BG==true) cout << "Fixed one\n";
					//Objective function value
					cost = 0;
					for (p = 0; p < R; p++) {
						b = yk[p][0];
						t = yk[p][1];
						s = yk[p][2];
						//if (p == R1) cout << endl;
						//cout << "Passenger: " << p << "\tBus_" << b << " Trip_" << t << " Stop_" << s;
						//continue;
						//Walking for all passengers
						cost += c2 * ttraveltimep[p][s];
						if (ttraveltimep[p][s] > dw) {
							INFEAS1 = true;
							break;
							//cout << "\twalking: " << int(traveltimep[p][s] / 60) << "**";
						}
						//else cout << "\twalking: " << int(traveltimep[p][s] / 60);
						in = false;
						//Travel for all passengers
						int xkS = xk[b][t].size() - 1;
						for (i = 0; i < xkS; i++) {
							if (xk[b][t][i] == s || in) {
								if (!in)l = i;
								in = true;
								cost += c1 * traveltimes[xk[b][t][i]][xk[b][t][i + 1]];
							}
						}
						if (s == N - 1) l = xk[b][t].size() - 1;
						//waiting
						//cout <<"Passenger " << p<<" Stop " << s << " index ";
						//cout << endl << l << "  " << xk[b][t].size() << endl;
						if (p < R1) {
							cost += c3 * abs(arrivals[p] - Dk[b][t].back());
							if (int(arrivals[p] - Dk[b][t].back()) > d_ae || int(Dk[b][t].back() - arrivals[p]) > d_al) {
								INFEAS2 = true;
								break;
								//cout << "\tdat: " << int((arrivals[p]) / 60) << " a: " << int(Dk[b][t].back() / 60) << "-> Diff: " << int((arrivals[p]) / 60) - int(Dk[b][t].back() / 60) <<"**" << endl;
							}
							//else {
								//cout << "\tdat: " << int((arrivals[p]) / 60) << " a: " << int(Dk[b][t].back() / 60) << "-> Diff: " << int((arrivals[p]) / 60) - int(Dk[b][t].back() / 60) << endl;
							//}
						}
						else {
							cost += c3 * abs(departures[p - R1] - Dk[b][t][l]);
							if (int(departures[p - R1] - Dk[b][t][l]) > d_de || int(Dk[b][t][l] - departures[p - R1]) > d_dl) {
								INFEAS3 = true;
								break;
								//cout << "\tedt: " << int((departures[p - R1]) / 60) << " d: " << int(Dk[b][t][l] / 60) << "-> Diff: " << int((departures[p - R1]) / 60) - int(Dk[b][t][l] / 60) <<"**" <<endl;
							}
							//else {
								//cout << "\tedt: " << int((departures[p - R1]) / 60) << " d: " << int(Dk[b][t][l] / 60) << "-> Diff: " << int((departures[p - R1]) / 60) - int(Dk[b][t][l] / 60) << endl;
							//}
						}
					}

					if (!INFEAS1 && !INFEAS2 && !INFEAS3) {
						//cout << "\n----------  Time between bus departures (min) ----------- \n";
						for (i = 0; i < N && !INFEAS4; i++) {
							IFC.clear();
							for (b = 0; b < B; b++) {
								int xkS1 = xk[b].size();
								for (t = 0; t < xkS1; t++) {
									if (t != 0) {
										if (Dk[b][t][0] < Dk[b][t - 1][Dk[b][t - 1].size() - 1] + short_route) {
											INFEAS5 = true;
											//cout << "transportation impossible\n";
											break;
										}
									}
									int xkS2 = xk[b][t].size();
									for (l = 0; l < xkS2; l++) {
										if (xk[b][t][l] == i)IFC.push_back(Dk[b][t][l]);
									}
								}
								if (INFEAS5) break;
							}
							if (INFEAS5) break;
							FCi = IFC.size();
							std::sort(IFC.begin(), IFC.begin() + FCi);
							//cout << "m_" << i << " -> ";
							for (j = 1; j < FCi; j++) {
								if (int(IFC[j] - IFC[j - 1]) > OGxt) {
									//cout << int((FC[i][j] - FC[i][j - 1]) / 60) << "** \t";
									INFEAS4 = true;
									break;
									//infs = " --> between " + to_string(int(FC[i][j - 1] / 60)) + " and " + to_string(int(FC[i][j] / 60));
								}
								//else {
									//cout << int((FC[i][j] - FC[i][j - 1]) / 60) << " \t";
								//}
							}
							//cout << endl;
						}
						//std::cout << "Elapsed time: " << elapsed_time << "s (initial solution)" << endl;
						//cout << "COST: " << cost << "s (initial solution)\n";
					}
					if (INFEAS1 || INFEAS2 || INFEAS3 || INFEAS4 || INFEAS5) {
						continue;
					}
				}

				//nhp = 0.05;
				//else logres += "cost: " + to_string(cost) + "\trun time: " + to_string(elapsed_time) + "\t";

				//logres += "   pm1: " + to_string(int(pm1 * 100)) + "%  pm2: " + to_string(int(pm2 * 100)) + "%  pm3: " + to_string(int(pm3 * 100)) + "%\n";
				//infeastemp = 0;
				//cout << "Found! one \n";
				//if (!(INFEAS1 || INFEAS2 || INFEAS3 || INFEAS4) && countp == R) {
					//countFeas++;
					//cout << cost << " Ts: " + to_string(Ts) << endl;
				//cout << " thread: " + to_string(omp_get_thread_num()) + " it: " + to_string(iii) + " cost: " + to_string(cost) + "\n";
#pragma omp critical
				{
					Costb.push_back(cost);
					//logresBest = logres;

					lPM1b.push_back(dPM10);
					lPM2b.push_back(dPM20);
					lPM3b.push_back(dPM30);
					lFPMb.push_back(dFPM0);
					lXTb.push_back(dXT0);
					lCb.push_back(dC0);
				}
				INFEASS0 = true;

				//}
				//if(INFEASS0)cout << "thread: "+ to_string(omp_get_thread_num())+" it: "+ to_string(iii)+" cost: "+to_string(cost)+" FEASS: " +to_string(INFEASS0)+"\n";
			}
			//cout << to_string(iii) + "\n";
		}
		best_cost = INT32_MAX;
		int b_c_i = -1;
		int MM = Costb.size();
		int run = MM;
		if (MM == 0) {
			cout << " --------------------- NO FEASIBLE SOLUTION FOUND ---------------------  \n";
			exit(0);
		}
		else cout << " Found " << MM << " feasible solutions at the start\n";
		for (int f = 0; f < MM; f++) {
			if (best_cost > Costb[f]) {
				best_cost = Costb[f];
				b_c_i = f;
			}
		}
		MM = lPM1b[b_c_i].size();
		lPM1.clear();
		lPM2.clear();
		lPM3.clear();
		lFPM.clear();
		lXT.clear();
		lC.clear();
		for (int i = 0; i < MM; i++) {
			lPM1.push_back(lPM1b[b_c_i][i]);
			lPM2.push_back(lPM2b[b_c_i][i]);
			lPM3.push_back(lPM3b[b_c_i][i]);
			lFPM.push_back(lFPMb[b_c_i][i]);
			lXT.push_back(lXTb[b_c_i][i]);
			lC.push_back(lCb[b_c_i][i]);
		}
		//cout << "Search now !!!\n";
		//#pragma omp parallel  for reduction(*:Ts)
		double cost_c = best_cost, dE = 0;
		//int run = 0;
		MM = int(lPM1.size() * 1.5);

		while (run < stop_it) {
			lPM1b.clear();
			lPM2b.clear();
			lPM3b.clear();
			lFPMb.clear();
			lXTb.clear();
			lCb.clear();

			MM = lPM1.size();
			Costb.clear();

			//cout << "+++++++++++++++++++++++++++ Search parallel again2\n";
			//int give0 = 0;
			std::atomic<bool> INFEASS(false);
#pragma omp parallel firstprivate(MM,traveltimes, ttraveltimep, arrivals, departures,tclosestPS,closestS, S,M,N,B,short_route, best_route,C,TS,OGxt,dw,d_dl,d_de,d_ae,d_al,c1,c2,c3,lPM1,lPM2,lPM3,lFPM,lXT,des)
			{
				//double countInfeas1 = 0, countInfeas2 = 0, countFeas = 0;
				//int s = seed[omp_get_thread_num()];
				default_random_engine& generator = generators[omp_get_thread_num()];
				//generator.seed((omp_get_thread_num()) * 800051150);
				//int iii, stoppp;
				int xt;
				double UBxt;
				//int LL = 30;

				int i, j, b, t, p, l, s;
				double pm1 = 1, pm2 = 1, pm3 = 1, fpm = 1;
				double b_next;
				vector<int> route;
				vector<float> dPM10;
				vector<float> dPM20;
				vector<float> dPM30;
				vector<float> dFPM0;
				vector<float> dXT0;
				vector<float> dC0;
				vector<double> IFC;
				int FCi = 0;
				int temp;
				//double dE;
				vector<double> timetable;
				vector<double> tempt;
				double timewindow, timewindow2;
				//double Arr;

				int trips[B]; //keeps track of which trip each bus is at needs to be updated
				int best_stop;
				double freqN[N];
				double newfreqN[N];

				double bd[B];
				int yk[R][3];

				double startopt;

				int countp, p1, p2, cp1, cp2;
				bool in, in2;
				double threshold, tt;

				double l_arr, l_dep;

				int b_it = 0;

				double minF = INT16_MAX, min_al = INT16_MAX, min_ae = INT16_MAX, min_dl = INT16_MAX, min_de = INT16_MAX;
				int nS, dst;
				double diffa1, diffa2, diffd1, diffd2, diffF;
				double difBD, maxFW, maxRW;//max time to arrive or depart earlier
				//add addtional passenger with DAT
				//*
				double extra = 0, b_extra = 0, xxt = 0;
				int indexpt[R1];
				int indexpt2[R2];
				double temparrivals[R1];
				double tempdept[R2];
				vector<vector<vector<int>>> xk(B);
				vector<vector<vector<double>>> Dk(B);
				double cost = 0, b_cost = 0;
				//int SL;
				bool INFEAS1 = false, INFEAS2 = false, INFEAS3 = false, INFEAS4 = false, INFEAS5 = false;
				int nLP = MM;
				int dSize = min(des, nLP - 1);
				uniform_int_distribution<int> destroy(0, nLP - 1);

				int* change1 = new int[dSize];
				int key1 = 0;
				//vector<int> change2;
				int* change2 = new int[dSize];
				int key2 = 0;
				//vector<int> change3;
				int* change3 = new int[dSize];
				int key3 = 0;
				//vector<int> change4;
				int* change4 = new int[dSize];
				int key4 = 0;
				//vector<int> change5;
				int* change5 = new int[dSize];
				int key5 = 0;

				int* change6 = new int[dSize];
				int key6 = 0;
				//change1.push_back(key1);
				//change2.push_back(key2);
				//change3.push_back(key3);
				//change4.push_back(key4);
				//change5.push_back(key5);
				//cout << lPM1.size() << endl;
				int* endC1 = change1 + dSize;
				int* endC2 = change2 + dSize;
				int* endC3 = change3 + dSize;
				int* endC4 = change4 + dSize;
				int* endC5 = change5 + dSize;
				int* endC6 = change6 + dSize;
				float Cc = 0.5;
				//cout << "check 1\n";

				//generator.seed(7851 * (omp_get_thread_num()));
				while (!INFEASS) {
					for (s = 0; s < B; s++) {
						xk[s].clear();
						Dk[s].clear();
					}
					//std::cout << "\n************************************************************************** Run number: " << run + 1 << endl;
					//logres = "";
					for (p = 0; p < R; p++) {
						yk[p][0] = -1;
						yk[p][1] = -1;
						yk[p][2] = -1;

					}
					for (p = 0; p < R1; p++) {
						indexpt[p] = p;
						temparrivals[p] = arrivals[p];
					}

					for (p = 0; p < R2; p++) {
						indexpt2[p] = p;
						tempdept[p] = departures[p];
					}
					quickSort(indexpt, temparrivals, 0, R1 - 1);
					quickSort(indexpt2, tempdept, 0, R2 - 1);
					best_stop = 0;
					startopt = tempdept[0] - 1000;
					threshold = 0, tt = 0;

					cost = 0, b_cost = 0;
					route.clear();

					timetable.clear();
					tempt.clear();
					//cout << "check\n";
					dPM10.clear();
					dPM20.clear();
					dPM30.clear();
					dFPM0.clear();
					dXT0.clear();
					dC0.clear();
					//cout << "  thread number: " + to_string(omp_get_thread_num()) + " loop number: " + to_string(iii + 1) + " \n";

					//vector<int> change1;
					key1 = destroy(generator);
					key2 = destroy(generator);
					key3 = destroy(generator);
					key4 = destroy(generator);
					key5 = destroy(generator);
					key6 = destroy(generator);
					change1[0] = key1;
					change2[0] = key2;
					change3[0] = key3;
					change4[0] = key4;
					change5[0] = key5;
					change6[0] = key6;
					//change1.push_back(key1);
					//change2.push_back(key2);
					//change3.push_back(key3);
					//change4.push_back(key4);
					//change5.push_back(key5);
					//cout << lPM1.size() << endl;
					for (i = 1; i < dSize; i++) {
						key1 = destroy(generator);
						while (std::find(change1, endC1, key1) != endC1) key1 = destroy(generator);
						change1[i] = key1;

						key2 = destroy(generator);
						while (std::find(change2, endC2, key2) != endC2) key2 = destroy(generator);
						change2[i] = key2;

						key3 = destroy(generator);
						while (std::find(change3, endC3, key3) != endC3) key3 = destroy(generator);
						change3[i] = key3;

						key4 = destroy(generator);
						while (std::find(change4, endC4, key4) != endC4) key4 = destroy(generator);
						change4[i] = key4;

						key5 = destroy(generator);
						while (std::find(change5, endC5, key5) != endC5) key5 = destroy(generator);
						change5[i] = key5;

						key6 = destroy(generator);
						while (std::find(change6, endC6, key6) != endC6) key6 = destroy(generator);
						change6[i] = key6;
					}
					for (b = 0; b < nLP; b++) {
						if (std::find(change1, endC1, b) != endC1) {
							normal_distribution<float> nPM1(lPM1[b], nhp);
							//cout << "check 2\n";
							pm1 = nPM1(generator);
							if (pm1 > 1)pm1 = 1;
							if (pm1 < 0)pm1 = 0;
							pm1 = round(pm1 * 20.0) / 20.0;
							dPM10.push_back(pm1);
						}
						else {
							pm1 = lPM1[b];
							dPM10.push_back(pm1);
						}
						if (std::find(change2, endC2, b) != endC2) {
							normal_distribution<float> nPM2(lPM2[b], nhp);

							pm2 = nPM2(generator);

							if (pm2 > 1)pm2 = 1;
							if (pm2 < 0)pm2 = 0;
							pm2 = round(pm2 * 20.0) / 20.0;
							dPM20.push_back(pm2);
						}
						else {
							pm2 = lPM2[b];
							dPM20.push_back(pm2);
						}
						if (std::find(change3, endC3, b) != endC3) {
							normal_distribution<float> nPM3(lPM3[b], nhp);

							pm3 = nPM3(generator);

							if (pm3 > 1)pm3 = 1;
							if (pm3 < 0)pm3 = 0;
							pm3 = round(pm3 * 20.0) / 20.0;
							dPM30.push_back(pm3);
						}
						else {
							pm3 = lPM3[b];
							dPM30.push_back(pm3);
						}
						if (std::find(change4, endC4, b) != endC4) {
							normal_distribution<float> nFPM(lFPM[b], nhp);

							fpm = nFPM(generator);

							if (fpm > 1)fpm = 1;
							if (fpm < 0)fpm = 0;
							fpm = round(fpm * 20.0) / 20.0;
							dFPM0.push_back(fpm);
						}
						else {
							fpm = lFPM[b];
							dFPM0.push_back(fpm);
						}
						if (std::find(change5, endC5, b) != endC5) {
							normal_distribution<float> nXT(lXT[b], nhp);
							xxt = nXT(generator);

							if (xxt > 1)xxt = 1;
							//if (xxt < 0.4)xxt = PlanB3(generator);

							if (xxt < 0.4)xxt = PlanB3(generator);

							xxt = round(xxt * 20.0) / 20.0;
							dXT0.push_back(xxt);
							xt = int(xxt * OGxt);
						}
						else {
							xxt = lXT[b];
							dXT0.push_back(xxt);
							xt = int(xxt * OGxt);
						}

						if (std::find(change6, endC6, b) != endC6) {
							normal_distribution<float> nC(lC[b], nhp);
							Cc = nC(generator);

							if (Cc > 1)Cc = 1;
							//if (xxt < 0.4)xxt = PlanB3(generator);

							if (Cc < 0.25)Cc = 0.25;

							Cc = round(Cc * 20.0) / 20.0;
							dC0.push_back(Cc);
							C = max(5, int(Cc * C_OG));
						}
						else {
							Cc = lC[b];
							dC0.push_back(Cc);
							C = max(5, int(Cc * C_OG));
						}
					}
					l_arr = arrivals[indexpt[0]], l_dep = departures[indexpt2[0]] + short_route * 0.75;
					//logres += "Run: " + to_string(run + 1) + "\t";
					//start_time = clock();
					//************************************Initial Solution***************************************
					for (i = 0; i < N; i++) {
						freqN[i] = -INT16_MAX;
					}
					countp = p1 = p2 = 0;
					for (b = 0; b < B; b++) {
						trips[b] = 0;
						bd[b] = startopt;//start of optimization
					}
					timetable.push_back(0);

					p1 = p2 = countp = 0;
					b = 0;
					b_next = 0.0;
					//cout << " END of PH: " << int(TS + startopt) / 60 << endl;
					b_it = -1;
					//cout << "  thread number: " + to_string(omp_get_thread_num()) + " loop number: " + to_string(iii + 1) + " \n";
					//if (!INFEASS) {

					while (timetable.back() < TS + startopt && p1 + p2 < R) {//until TS is over+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++	
						b_it++;
						if (b_it < nLP) {
							pm1 = dPM10[b_it];
							pm2 = dPM20[b_it];
							pm3 = dPM30[b_it];
							fpm = dFPM0[b_it];
							xt = int(dXT0[b_it] * OGxt);
							C = max(5, int(dC0[b_it] * C_OG));
						}
						else {
							pm1 = PM(generator);
							pm2 = PM(generator);
							pm3 = PM(generator);
							fpm = PM(generator);

							if (pm1 > 1)pm1 = 1;
							if (pm1 < 0)pm1 = 0;
							if (pm2 > 1)pm2 = 1;
							if (pm2 < 0)pm2 = 0;
							if (pm3 > 1)pm3 = 1;
							if (pm3 < 0)pm3 = 0;
							if (fpm > 1)fpm = 1;
							if (fpm < 0)fpm = 0;

							UBxt = gXT(generator);
							xt = int(UBxt * OGxt);

							dPM10.push_back(pm1);
							dPM20.push_back(pm2);
							dPM30.push_back(pm3);
							dFPM0.push_back(fpm);
							dXT0.push_back(UBxt);

							float Cc = gC(generator);
							dC0.push_back(Cc);

							C = max(5, int(Cc * C_OG));
						}


						//xt = OGxt;
						//if (infeastemp > 100) fpm = 1;
						//cout << " xt: " << xt / 60 << endl;
						//determine which bus is available first
						b = iMin(bd, B);
						b_next = iMin2(bd, B);
						//cout << "++++++ || Bus: " << b << " bd: " << int(bd[b] / 60) << " trip: " << trips[b] << " || ++++++\n";
						//for (i = 0; i < B; i++) cout << int(bd[i] / 60) << " ";
						//cout << endl;
						//cout << "bd1: " << int(bd[b]/60) << " bd2: " << int(b_next/60) << endl;
						//Make route only with mandatory stops, ASAP
						route.clear();
						timetable.clear();
						tempt.clear();
						tt = bd[b];
						timetable.push_back(tt);
						route.push_back(0);
						for (i = 1; i < N; i++) {
							route.push_back(i);
							tt += traveltimes[i - 1][i];
							timetable.push_back(tt);
						}
						//Assignement: First look at R1
						timewindow = timewindow2 = INT32_MAX;
						cp1 = cp2 = 0;

						//++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++ CASE I ++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
						if (l_arr < l_dep && p1 < R1 || p2 == R2) {
							//cout << "R1" << endl;
							p = 0;
							while (cp1 + cp2 < C && countp < R && p < R1 && (int)(arrivals[indexpt[p]] - timewindow) <= d_ae + d_al) {
								//cout << " check start\n";
								//cout << cp1 + cp2 << "<" << C << "  " << countp << "<" << R << "  " << (int)(arrivals[indexpt[p]] - timewindow) << "< " << d_ae + d_al << endl;
								if (ttraveltimep[indexpt[p]][tclosestPS[indexpt[p]][0]] > dw) {
									std::cout << " Infeasible solution, for walking times " << endl;
									exit(0);
								}
								if (temparrivals[indexpt[p]] == -1) {
									//cout << "passenger already in solution \n";
									//cout << "p: " << p << " indexpt: " << indexpt[p] << endl;
									p++;
									if (p > R1)break;
									continue;//if this is already in the solution, continue to next passenger
								}
								if (cp1 == 0)timewindow = arrivals[indexpt[p]];

								//Assign bus stop to passenger
								//choose best stop and update route
								if (cp1 == 0) {
									//cout << " check 2 \n";
									best_stop = bestStop(timetable, route, traveltimes, ttraveltimep, tclosestPS, indexpt[p], N,
										dw, best_route, M, S, bd[b], freqN, xt, best_stop, arrivals[indexpt[p]],
										arrivals[indexpt[p]], d_ae, d_al, yk, arrivals, R1, R2, b, trips[b], b_next, fpm);
									//cout << " check stop 0\n";
									if (best_stop == -1) {
										//cout << "passenger " << indexpt[p] << " --> Skip this one, not good for bus stop\n";
										p++;
										//cout << "Next passenger " << p1 << endl;
										if (p > R1)break;
										continue;
									}
									//cout << " --- FIRST passenger added : p= " << indexpt[p] << "\n";
									//Assign bus to passenger
									yk[indexpt[p]][0] = b;
									yk[indexpt[p]][1] = trips[b];
									yk[indexpt[p]][2] = best_stop;
									temparrivals[indexpt[p]] = -1;
									tempt.push_back(arrivals[indexpt[p]]);
									p++;
									p1++;
									cp1++;
									countp++;
									if (p > R1)break;
								}
								else {
									temp = bestStop(timetable, route, traveltimes, ttraveltimep, tclosestPS, indexpt[p], N, dw,
										best_route, M, S, bd[b], freqN, xt, best_stop, tempt.back(),
										arrivals[indexpt[p]], d_ae, d_al, yk, arrivals, R1, R2, b, trips[b], b_next, fpm);
									//cout << " check stop n: " << temp <<"\n";
									if (temp == -1) {
										//cout << "passenger " << indexpt[p] << " --> Skip this one, not good for bus stop\n";
										p++;//next stop of a passenger with later edt is before a previous stop with a passenger with earlier edt, not possible OR edts are too different
										if (p > R1)break;
										continue;
									}
									else {
										// time to go from prev to next stop on a full route 
										best_stop = temp;
										//Assign bus to passenger
										yk[indexpt[p]][0] = b;
										yk[indexpt[p]][1] = trips[b];
										yk[indexpt[p]][2] = best_stop;
										temparrivals[indexpt[p]] = -1;
										tempt.push_back(arrivals[indexpt[p]]);
										p++;
										p1++;
										cp1++;
										countp++;
										if (p > R1)break;
										//cout << "-- NEXT passenger added : p= " << indexpt[p] << "\n";
										//cout << " indexpt: " << indexpt[p] << " arrivals p: " << arrivals[indexpt[p]] <<  endl;
										//cout << " p: " << p << " indexpt: " << indexpt[p] << endl;
									}
								}
							}
							//cout << " check end\n";
							//cout << "timetable:" << timetable.size() << " route: " << route.size() << " Passengers added: " << tempt.size() << endl;

						}
						// //++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++ CASE II ++++++++++++++++++++++++++++++++++++++++++++ Now look at R2
						//*
						if ((l_arr >= l_dep && p2 < R2) || cp1 == 0) {
							//cout << "R2" << endl;
							//cout << timetable.size() << endl;
							timetable.clear();
							tt = bd[b];
							timetable.push_back(tt);
							for (i = 1; i < N; i++) {
								tt += traveltimes[i - 1][i];
								timetable.push_back(tt);
							}

							p = 0;
							while (cp2 < C && countp < R && p < R2) {
								//cout << p2 << "<" << C << "  " << countp << "<" << R << " " << p << "< " << R2 << endl;
								//cout << "passenger " << indexpt2[p] + R1 << "  ------------- \n";
								if (ttraveltimep[R1 + indexpt2[p]][tclosestPS[R1 + indexpt2[p]][0]] > dw) {
									std::cout << " Infeasible solution, for walking times " << endl;
									exit(0);
								}
								if (tempdept[indexpt2[p]] == -1) {
									//cout << "passenger already in solution \n";
									p++;
									continue;//if this is already in the solution, continue to next passenger
								}
								t = 0;
								int ttS = timetable.size();
								for (i = 0; i < ttS; i++) {
									if (route[i] < N) {
										newfreqN[t] = timetable[i];
										t++;
									}
								}
								if (cp2 == 0) {
									best_stop = bestStop2(timetable, route, traveltimes, ttraveltimep, tclosestPS, indexpt2[p] + R1, N,
										dw, best_route, M, S, bd[b], freqN, newfreqN, xt, best_stop, departures[indexpt2[p]],
										departures[indexpt2[p]], d_de, d_dl, yk, departures, R1, R2, b, trips[b], b_next, fpm);
									if (best_stop == -1) {
										//cout << "passenger " << indexpt2[p] + R1 << " --> Skip this one, not good for bus stop\n";
										p++;
										//cout << "Next passenger " << p1 << endl;
										continue;
									}
									//cout << " --- FIRST passenger added : p= " << indexpt2[p] + R1 << "\n";
									//Assign bus to passenger
									yk[R1 + indexpt2[p]][0] = b;
									yk[R1 + indexpt2[p]][1] = trips[b];
									//Assign bus stop to passenger
									//choose best stop and update route
									//cout << "  Best stop: " << best_stop << endl;
									//cout << "Passenger: " << R1 + indexpt2[p] << " edt: " << int(departures[indexpt2[p]] / 60) << " stop: " << best_stop << endl;
									tempt.push_back(departures[indexpt2[p]]);
									tempdept[indexpt2[p]] = -1;
									yk[R1 + indexpt2[p]][2] = best_stop;
									cp2++;
									p2++;
									countp++;
									p++;
								}
								else {
									temp = bestStop2(timetable, route, traveltimes, ttraveltimep, tclosestPS, indexpt2[p] + R1, N, dw,
										best_route, M, S, bd[b], freqN, newfreqN, xt, best_stop, tempt.back(),
										departures[indexpt2[p]], d_de, d_dl, yk, departures, R1, R2, b, trips[b], b_next, fpm);

									if (temp == -1) {
										//cout << "Passenger: " << R1 + indexpt2[p] << " edt: "<< int(departures[indexpt2[p]] / 60) << "Skip this one, not good for bus stop\n";
										p++;//next stop of a passenger with later edt is before a previous stop with a passenger with earlier edt, not possible OR edts are too different
										continue;
									}
									else {
										// time to go from prev to next stop on a full route 
										best_stop = temp;
										//Assign bus to passenger
										yk[R1 + indexpt2[p]][0] = b;
										yk[R1 + indexpt2[p]][1] = trips[b];
										//Assign bus stop to passenger
										yk[R1 + indexpt2[p]][2] = best_stop;
										//cout << "Passenger: " << R1 + indexpt2[p] << " edt: " << int(departures[indexpt2[p]] / 60) << " stop: " << best_stop << endl;
										tempt.push_back(departures[indexpt2[p]]);
										tempdept[indexpt2[p]] = -1;
										cp2++;
										p2++;
										countp++;
										p++;
										//cout << "-- NEXT passenger added : p= " << indexpt2[p] + R1 << "\n";
									}
								}
							}
							int temptS = tempt.size();
							if (p1 < R1 && l_arr >= l_dep && temptS == 0) {
								//++++++++++++++++++++++++++++++++ CASE I ++++++++++++++++++++++++++++++++++++++++++++
								//cout << "R1 again" << endl;
								p = 0;
								while (cp1 + cp2 < C && countp < R && p < R1 && (int)(arrivals[indexpt[p]] - timewindow) <= d_ae + d_al) {
									//cout << cp1 + cp2 << "<" << C << "  " << countp << "<" << R << "  " << (int)(arrivals[indexpt[p]] - timewindow) << "< " << d_ae + d_al << endl;
									if (ttraveltimep[indexpt[p]][tclosestPS[indexpt[p]][0]] > dw) {
										std::cout << " Infeasible solution, for walking times " << endl;
										exit(0);
									}
									if (temparrivals[indexpt[p]] == -1) {
										//cout << "passenger already in solution \n";
										//cout << "p: " << p << " indexpt: " << indexpt[p] << endl;
										p++;
										if (p > R1)break;
										continue;//if this is already in the solution, continue to next passenger
									}
									if (cp1 == 0)timewindow = arrivals[indexpt[p]];
									//Assign bus stop to passenger
									//choose best stop and update route
									if (cp1 == 0) {
										best_stop = bestStop(timetable, route, traveltimes, ttraveltimep, tclosestPS, indexpt[p], N,
											dw, best_route, M, S, bd[b], freqN, xt, best_stop, arrivals[indexpt[p]],
											arrivals[indexpt[p]], d_ae, d_al, yk, arrivals, R1, R2, b, trips[b], b_next, fpm);
										//cout << " check\n";
										if (best_stop == -1) {
											//cout << "passenger " << indexpt[p]  << " --> Skip this one, not good for bus stop\n";
											p++;
											//cout << "Next passenger " << p1 << endl;
											if (p > R1)break;
											continue;
										}
										//cout << " --- FIRST passenger added\n";
										//Assign bus to passenger
										yk[indexpt[p]][0] = b;
										yk[indexpt[p]][1] = trips[b];
										yk[indexpt[p]][2] = best_stop;
										temparrivals[indexpt[p]] = -1;
										tempt.push_back(arrivals[indexpt[p]]);
										p++;
										p1++;
										cp1++;
										countp++;
										if (p > R1)break;
										//cout << " --- FIRST passenger added : p= " << indexpt[p] << "\n";
									}
									else {
										temp = bestStop(timetable, route, traveltimes, ttraveltimep, tclosestPS, indexpt[p], N, dw,
											best_route, M, S, bd[b], freqN, xt, best_stop, tempt.back(),
											arrivals[indexpt[p]], d_ae, d_al, yk, arrivals, R1, R2, b, trips[b], b_next, fpm);
										//cout << " check\n";
										if (temp == -1) {
											//cout << "passenger " << indexpt[p] << " --> Skip this one, not good for bus stop\n";
											p++;//next stop of a passenger with later edt is before a previous stop with a passenger with earlier edt, not possible OR edts are too different
											if (p > R1)break;
											continue;

										}
										else {
											// time to go from prev to next stop on a full route 
											best_stop = temp;
											//Assign bus to passenger
											yk[indexpt[p]][0] = b;
											yk[indexpt[p]][1] = trips[b];
											yk[indexpt[p]][2] = best_stop;
											temparrivals[indexpt[p]] = -1;
											tempt.push_back(arrivals[indexpt[p]]);
											p++;
											p1++;
											cp1++;
											countp++;
											if (p > R1)break;
											//cout << "-- NEXT passenger added : p= " << indexpt[p] << "\n";
										}
									}

								}

							}
							//cout << "timetable:" << timetable.size() << " route: " << route.size() << " Passengers added: " << tempt.size() << endl;
						}
						//*/
						int temptS = tempt.size();
						if (temptS == 0) {
							timetable.clear();
							timetable.push_back(max(freqN[0] + xt, bd[b]));
							//timetable.push_back(freqN[0] + xt);
							tt = -1;
							for (i = 1; i < N; i++) {
								timetable.push_back(timetable[i - 1] + traveltimes[i - 1][i]);
								if (timetable[i] - freqN[i] > tt)tt = timetable[i] - freqN[i];
							}
							if (tt > xt) {
								for (i = 0; i < N; i++) {
									timetable[i] -= (tt - xt);
								}
							}
						}
						else {
							//add addtional passenger with DAT

							minF = INT16_MAX, min_al = INT16_MAX, min_ae = INT16_MAX, min_dl = INT16_MAX, min_de = INT16_MAX;
							nS = timetable.size() - 1;

							for (p = 0; p < R1; p++) {
								if (yk[p][0] == b && yk[p][1] == trips[b]) {
									//cout << "passenger " << p  << ": dat=" << int(arrivals[p] / 60) << " arr=" << int(timetable[nS] / 60) << endl;
									diffa1 = d_ae - (arrivals[p] - timetable[nS]);
									diffa2 = d_al - (timetable[nS] - arrivals[p]);
									if (arrivals[p] >= timetable[nS] && min_ae > diffa1) {
										min_ae = diffa1;
									}
									if (arrivals[p] <= timetable[nS] && min_al > diffa2) {
										min_al = diffa2;
									}
								}
							}

							dst = -1;
							int routeS = route.size(), timetableS = timetable.size();
							for (p = 0; p < R2; p++) {
								dst = -1;
								for (i = 0; i < routeS; i++) {
									if (yk[p + R1][0] == b && yk[p + R1][1] == trips[b] && yk[p + R1][2] == route[i]) {
										dst = i;
										break;
									}
								}
								if (dst != -1) {
									//cout << "passenger " << p +R1 << ": edt=" << int(departures[p] / 60) << " dept=" << int(timetable[dst] / 60) << " at stop: " <<route[dst] << endl;
									diffd1 = d_de - (departures[p] - timetable[dst]);
									diffd2 = d_dl - (timetable[dst] - departures[p]);
									if (departures[p] >= timetable[dst] && min_de > diffd1) {
										min_de = diffd1;
									}
									if (departures[p] <= timetable[dst] && min_dl > diffd2) {
										min_dl = diffd2;
									}
								}
							}
							if (freqN[0] != -INT16_MAX) {
								for (i = 0; i < timetableS; i++) {
									if (route[i] < N && freqN[route[i]] < timetable[i]) {
										diffF = xt - (timetable[i] - freqN[route[i]]);
										if (minF > diffF) minF = diffF;
									}
								}
							}

							//cout << " min diff al: " << int(min_al/60) << " min diff dl: " << int(min_dl/60) << " min diff Freq: " << int(minF / 60) << endl;
							//cout << " min diff ae: " << int(min_ae/60) << " min diff de: " << int(min_de/60) << endl;
							difBD = timetable[0] - bd[b];
							maxFW = min(min(min_al, min_dl), minF) * pm1; // max time to depart later 
							maxRW = min(min(min_ae, min_de), difBD) * pm2;//max time to arrive or depart earlier
							//add addtional passenger with DAT
							//*
							extra = 0;
							//cout << "Max time FW: " << int(maxFW/60) << " max time RW: " << int(maxRW/60) << "\nStart R1\n";
							//cout << "R1: " << cp1 << " R2: " << cp2 << endl;

							for (p = 0; p < R1; p++) {
								if (temparrivals[p] != -1) {
									//cout << "for p_" << p << " dat: "<<arrivals[p]/60<< " arrval: " << timetable.back()/60<<endl;
									i = 0;
									b_cost = INT32_MAX;
									s = -1;
									if (arrivals[p] >= timetable.back()) {
										if (arrivals[p] - timetable.back() <= d_ae) extra = 0;
										else extra = (arrivals[p] - timetable.back()) - d_ae;
									}
									else {
										if (timetable.back() - arrivals[p] <= d_al) extra = 0;
										else extra = d_al - (timetable.back() - arrivals[p]);
									}
									while (ttraveltimep[p][tclosestPS[p][i]] < dw * pm3) {
										//cout << "try stop: " << tclosestPS[p][i] << endl;
										routeS = route.size() - 1;
										for (t = 0; t < routeS; t++) {
											in = false;
											if (tclosestPS[p][i] == route[t]) {
												in = true;
												break;
											}
										}
										in2 = false;
										if (in && arrivals[p] - timetable.back() - maxFW <= d_ae && timetable.back() - maxRW - arrivals[p] <= d_al) {
											in2 = true;
										}

										if (in2 && cp1 + cp2 < C) {
											//cout << " ++++++++++++ passenger " << p << " (dat=" << int(arrivals[p] / 60) << ") stop " << tclosestPS[p][i] << " (tt=" << int(timetable.back() / 60) << ") possible extra: " << int(extra / 60) << endl;
											cost = c2 * ttraveltimep[p][tclosestPS[p][i]] + c3 * abs(timetable.back() - arrivals[p]) + c3 * (cp1 + cp2) * (abs(extra));
											if (b_cost > cost) {
												b_cost = cost;
												s = route[t];
											}
										}
										i++;
									}
									if (b_cost != INT32_MAX) {
										//cout << "Added DAT \t";
										//cout << "Passenger: " << p << " arr: " << arrivals[p] / 60 << endl;
										yk[p][0] = b;
										yk[p][1] = trips[b];
										yk[p][2] = s;
										//cout << "Passenger: " << R1 + p << endl;
										temparrivals[p] = -1;//indicate this passenger is onboard already
										p1++;
										cp1++;
										countp++;
										//break;

										//update timetable
										//cout << " move tt with " << int(extra / 60) << endl;
										timetableS = timetable.size();
										for (l = 0; l < timetableS; l++) {
											timetable[l] += extra;
										}
										//update intermediary parameters
										minF = INT16_MAX, min_al = INT16_MAX, min_ae = INT16_MAX, min_dl = INT16_MAX, min_de = INT16_MAX;
										nS = timetable.size() - 1;
										for (int p11 = 0; p11 < R1; p11++) {
											if (yk[p11][0] == b && yk[p11][1] == trips[b]) {
												diffa1 = d_ae - (arrivals[p11] - timetable[nS]);
												diffa2 = d_al - (timetable[nS] - arrivals[p11]);
												if (arrivals[p11] >= timetable[nS] && min_ae > diffa1) {
													min_ae = diffa1;
												}
												if (arrivals[p11] <= timetable[nS] && min_al > diffa2) {
													min_al = diffa2;
												}
											}
										}
										for (int p22 = 0; p22 < R2; p22++) {
											dst = -1;
											routeS = route.size();
											for (l = 0; l < routeS; l++) {
												if (yk[p22 + R1][0] == b && yk[p22 + R1][1] == trips[b] && yk[p22 + R1][2] == route[l]) {
													dst = l;
													break;
												}
											}
											if (dst != -1) {
												diffd1 = d_de - (departures[p22] - timetable[dst]);
												diffd2 = d_dl - (timetable[dst] - departures[p22]);
												if (departures[p22] >= timetable[dst] && min_de > diffd1) {
													min_de = diffd1;
												}
												if (departures[p22] <= timetable[dst] && min_dl > diffd2) {
													min_dl = diffd2;
												}
											}
										}
										if (freqN[0] != -INT16_MAX) {
											timetableS = timetable.size();
											for (l = 0; l < timetableS; l++) {
												if (route[l] < N && freqN[route[l]] < timetable[l]) {
													diffF = xt - (timetable[l] - freqN[route[l]]);
													if (minF > diffF) minF = diffF;
												}
											}
										}
										//cout << " min diff al: " << int(min_al/60) << " min diff dl: " << int(min_dl/60) << endl;
										//cout << " min diff ae: " << int(min_ae/60) << " min diff de: " << int(min_de/60) << " min diff Freq: " << int(minF/60) << endl;
										difBD = timetable[0] - bd[b];
										maxFW = min(min(min_al, min_dl), minF) * pm1; // max time to depart later 
										maxRW = min(min(min_ae, min_de), difBD) * pm2;//max time to arrive or depart earlier
										//cout << "  Added passenger " << p << " --> NEW Max time FW: " << int(maxFW/60) << " max time RW: " << int(maxRW/60) << endl;

									}
								}
							}

							//add addtional passenger with EDT
							///*
							//cout << "Start R2\nNEW Max time FW : " << int(maxFW / 60) << " max time RW : " << int(maxRW / 60) << endl;
							b_extra = 0;
							for (p = 0; p < R2; p++) {
								if (tempdept[p] != -1) {
									//cout << "try p_" << p+R1 << " dept: "<<departures[p]/60<<endl;
									i = 0;
									b_cost = INT32_MAX;
									s = -1;
									while (ttraveltimep[R1 + p][tclosestPS[p + R1][i]] < dw * pm3) {
										routeS = route.size() - 1;
										//if (R1 + p == 36)cout << tclosestPS[p + R1][i] << endl;
										for (t = 0; t < routeS; t++) {
											in = false;
											if (tclosestPS[p + R1][i] == route[t]) {
												//cout << "bus stop " << tclosestPS[p + R1][i] << " position " << t << endl;
												in = true;
												break;
											}
										}
										in2 = false;
										if (in && departures[p] - timetable[t] - maxFW <= d_de && timetable[t] - maxRW - departures[p] <= d_dl) {
											in2 = true;
										}
										//if (in2) cout << "bus stop " << tclosestPS[p + R1][i] << " position " << t << " length of tt: " << timetable.size() << endl;

										if (in2 && cp1 + cp2 < C) {
											//cout << "bus stop " << tclosestPS[p + R1][i] << " position " << t << " length of tt: " << timetable.size() << endl;
											if (departures[p] >= timetable[t]) {
												if (departures[p] - timetable[t] <= d_de)extra = 0;
												else extra = (departures[p] - timetable[t]) - d_de;
											}
											else {
												if (timetable[t] - departures[p] <= d_dl)extra = 0;
												else extra = d_dl - (timetable[t] - departures[p]);
											}
											//cout << "walking time " << traveltimep[R1 + p][tclosestPS[p + R1][i]] / 60 << endl;
											//cout << " ++++++++++++ passenger " << p+R1 << " (edt="<< int(departures[p]/60) << ") stop " << tclosestPS[p + R1][i] <<" (tt="<< int(timetable[t]/60) << ") possible extra: " << int(extra / 60)<< endl;
											cost = c2 * ttraveltimep[R1 + p][tclosestPS[p + R1][i]] + c3 * abs(timetable[t] - departures[p]) + c3 * (cp1 + cp2) * (abs(extra));
											if (b_cost > cost) {
												b_cost = cost;
												s = route[t];
												b_extra = extra;
											}
										}
										i++;
									}
									if (b_cost != INT32_MAX) {
										//cout << "Added EDT \t";
										//cout << "Passenger: " << R1 + p << " dept: " << departures[p] / 60 << endl;
										yk[p + R1][0] = b;
										yk[p + R1][1] = trips[b];
										yk[p + R1][2] = s;
										//cout << "Passenger: " << R1 + p << endl;
										tempdept[p] = -1;//indicate this passenger is onboard already
										p2++;
										cp2++;
										countp++;
										//break;
										//update timetable
										//cout << " move tt with " << int(b_extra / 60) << endl;
										timetableS = timetable.size();
										for (l = 0; l < timetableS; l++) {
											timetable[l] += b_extra;
										}
										//update intermediary parameters
										minF = INT16_MAX, min_al = INT16_MAX, min_ae = INT16_MAX, min_dl = INT16_MAX, min_de = INT16_MAX;
										nS = timetable.size() - 1;
										for (int p11 = 0; p11 < R1; p11++) {
											if (yk[p11][0] == b && yk[p11][1] == trips[b]) {
												diffa1 = d_ae - (arrivals[p11] - timetable[nS]);
												diffa2 = d_al - (timetable[nS] - arrivals[p11]);
												if (arrivals[p11] >= timetable[nS] && min_ae > diffa1) {
													min_ae = diffa1;
												}
												if (arrivals[p11] <= timetable[nS] && min_al > diffa2) {
													min_al = diffa2;
												}
											}
										}
										for (int p22 = 0; p22 < R2; p22++) {
											dst = -1;
											routeS = route.size();
											for (l = 0; l < routeS; l++) {
												if (yk[p22 + R1][0] == b && yk[p22 + R1][1] == trips[b] && yk[p22 + R1][2] == route[l]) {
													dst = l;
													break;
												}
											}
											if (dst != -1) {
												diffd1 = d_de - (departures[p22] - timetable[dst]);
												diffd2 = d_dl - (timetable[dst] - departures[p22]);
												if (departures[p22] >= timetable[dst] && min_de > diffd1) {
													min_de = diffd1;
												}
												if (departures[p22] <= timetable[dst] && min_dl > diffd2) {
													min_dl = diffd2;
												}
											}
										}
										if (freqN[0] != -INT16_MAX) {
											timetableS = timetable.size();
											for (l = 0; l < timetableS; l++) {
												if (route[l] < N && freqN[route[l]] < timetable[l]) {
													diffF = xt - (timetable[l] - freqN[route[l]]);
													if (minF > diffF) minF = diffF;
												}
											}
										}

										difBD = timetable[0] - bd[b];
										maxFW = min(min(min_al, min_dl), minF) * pm1; // max time to depart later 
										maxRW = min(min(min_ae, min_de), difBD) * pm2;//max time to arrive or depart earlier
										//cout << "    min diff al: " << int(min_al/60) << " min diff dl: " << int(min_dl/60) << endl;
										//cout << "    min diff ae: " << int(min_ae/60) << " min diff de: " << int(min_de/60) << " min diff Freq: " << int(minF/60) << endl;
										//cout << "  Added passenger " << p + R1 << " --> NEW Max time FW: " << int(maxFW / 60) << " max time RW: " << int(maxRW / 60) << endl;
									}
								}
							}
							//cout << "AFTER --> R1: " << cp1 << " R2: " << cp2 << endl;
							//*/

						}
						//cout << "R1+R2 done  Nroute: " << route.size() << " N_timetable: " << timetable.size() << endl;
						//update trip of bus b
						trips[b]++;
						//update bd of bus b
						bd[b] = timetable.back() + short_route;
						//update varaibles
						xk[b].push_back(route);
						Dk[b].push_back(timetable);
						//update frequency at mandatory 
						//cout << "xt constraints: \n";
						//cout << " ******* TIMETABLE: ";
						int timetableS = timetable.size();
						for (i = 0; i < timetableS; i++) {
							if (route[i] < N && (freqN[route[i]] < timetable[i] || freqN[route[i]] - timetable[i] > xt)) {
								//if(timetable[i] - freqN[route[i]]+1>xt) cout << int((timetable[i]- freqN[route[i]])) / 60 << " (s: "<< route[i]<< ") ";
								freqN[route[i]] = timetable[i];
								t++;
							}
							//cout << int(timetable[i] / 60) << " ";
						}
						//cout << endl << " FreqN: " ;
						//for (i = 0; i < N; i++) {
							//cout << int(freqN[i] / 60) << "  ";
						//}
						//cout << endl;
						/*
						cout << " timetable: \n";
						for (i = 0; i < timetable.size(); i++) {
							cout << int(timetable[i] / 60) << " ";
						}
						cout << endl;
						//*/
						//cout << "Passengers added to bus: " << cp1 + cp2 << endl;
						//cout << endl << "Number of passenger assigned: R2: " << p2 << " R1: " << p1 << endl << endl;
						if (p2 < R2) {
							for (p = 0; p < R2; p++) {
								if (tempdept[indexpt2[p]] != -1) {
									l_dep = departures[indexpt2[p]] + 0.75 * short_route;
									break;
								}
							}
						}
						if (p1 < R1) {
							for (p = 0; p < R1; p++) {
								if (temparrivals[indexpt[p]] != -1) {
									l_arr = arrivals[indexpt[p]];
									break;
								}
							}
						}
					}
					//elapsed_time = (double)(clock() - start_time) / CLK_TCK;
					//cout << "  thread number: " + to_string(omp_get_thread_num()) + " loop number: " + to_string(iii + 1) + " \n";
					//iii++;
					INFEAS1 = false, INFEAS2 = false, INFEAS3 = false, INFEAS4 = false, INFEAS5 = false;
					if (countp != R) {
						//countInfeas1++;
						//infeastemp++;
						continue;
					}
					else {
						//if(BG==true) cout << "Fixed one\n";
						//Objective function value
						cost = 0;
						for (p = 0; p < R; p++) {
							b = yk[p][0];
							t = yk[p][1];
							s = yk[p][2];
							//if (p == R1) cout << endl;
							//cout << "Passenger: " << p << "\tBus_" << b << " Trip_" << t << " Stop_" << s;
							//continue;
							//Walking for all passengers
							cost += c2 * ttraveltimep[p][s];
							if (ttraveltimep[p][s] > dw) {
								INFEAS1 = true;
								break;
								//cout << "\twalking: " << int(traveltimep[p][s] / 60) << "**";
							}
							//else cout << "\twalking: " << int(traveltimep[p][s] / 60);
							in = false;
							//Travel for all passengers
							int xkS = xk[b][t].size() - 1;
							for (i = 0; i < xkS; i++) {
								if (xk[b][t][i] == s || in) {
									if (!in)l = i;
									in = true;
									cost += c1 * traveltimes[xk[b][t][i]][xk[b][t][i + 1]];
								}
							}
							if (s == N - 1) l = xk[b][t].size() - 1;
							//waiting
							//cout <<"Passenger " << p<<" Stop " << s << " index ";
							//cout << endl << l << "  " << xk[b][t].size() << endl;
							if (p < R1) {
								cost += c3 * abs(arrivals[p] - Dk[b][t].back());
								if (int(arrivals[p] - Dk[b][t].back()) > d_ae || int(Dk[b][t].back() - arrivals[p]) > d_al) {
									INFEAS2 = true;
									break;
									//cout << "\tdat: " << int((arrivals[p]) / 60) << " a: " << int(Dk[b][t].back() / 60) << "-> Diff: " << int((arrivals[p]) / 60) - int(Dk[b][t].back() / 60) <<"**" << endl;
								}
								//else {
									//cout << "\tdat: " << int((arrivals[p]) / 60) << " a: " << int(Dk[b][t].back() / 60) << "-> Diff: " << int((arrivals[p]) / 60) - int(Dk[b][t].back() / 60) << endl;
								//}
							}
							else {
								cost += c3 * abs(departures[p - R1] - Dk[b][t][l]);
								if (int(departures[p - R1] - Dk[b][t][l]) > d_de || int(Dk[b][t][l] - departures[p - R1]) > d_dl) {
									INFEAS3 = true;
									break;
									//cout << "\tedt: " << int((departures[p - R1]) / 60) << " d: " << int(Dk[b][t][l] / 60) << "-> Diff: " << int((departures[p - R1]) / 60) - int(Dk[b][t][l] / 60) <<"**" <<endl;
								}
								//else {
									//cout << "\tedt: " << int((departures[p - R1]) / 60) << " d: " << int(Dk[b][t][l] / 60) << "-> Diff: " << int((departures[p - R1]) / 60) - int(Dk[b][t][l] / 60) << endl;
								//}
							}
						}

						if (!INFEAS1 && !INFEAS2 && !INFEAS3) {
							//vector<vector<double>> FC(N);
							//cout << "\n----------  Time between bus departures (min) ----------- \n";
							for (i = 0; i < N && !INFEAS4; i++) {
								IFC.clear();
								for (b = 0; b < B; b++) {
									int xkS1 = xk[b].size();
									for (t = 0; t < xkS1; t++) {
										if (t != 0) {
											if (Dk[b][t][0] < Dk[b][t - 1][Dk[b][t - 1].size() - 1] + short_route) {
												INFEAS5 = true;
												//cout << "transportation impossible\n";
												break;
											}
										}
										int xkS2 = xk[b][t].size();
										for (l = 0; l < xkS2; l++) {
											if (xk[b][t][l] == i)IFC.push_back(Dk[b][t][l]);
										}
									}
									if (INFEAS5) break;
								}
								if (INFEAS5) break;
								FCi = IFC.size();
								std::sort(IFC.begin(), IFC.begin() + FCi);
								//cout << "m_" << i << " -> ";
								for (j = 1; j < FCi; j++) {
									if (int(IFC[j] - IFC[j - 1]) > OGxt) {
										//cout << int((FC[i][j] - FC[i][j - 1]) / 60) << "** \t";
										INFEAS4 = true;
										break;
										//infs = " --> between " + to_string(int(FC[i][j - 1] / 60)) + " and " + to_string(int(FC[i][j] / 60));
									}
									//else {
										//cout << int((FC[i][j] - FC[i][j - 1]) / 60) << " \t";
									//}
								}
								//cout << endl;
							}
							//std::cout << "Elapsed time: " << elapsed_time << "s (initial solution)" << endl;
							//cout << "COST: " << cost << "s (initial solution)\n";
						}
						if (INFEAS1 || INFEAS2 || INFEAS3 || INFEAS4 || INFEAS5) {
							//countInfeas2++;
							//infeastemp++;
							//nhp = 0.02;
							continue;
						}
					}
					//if (!(INFEAS1 || INFEAS2 || INFEAS3 || INFEAS4) && countp == R) {
#pragma omp critical
					{
						Costb.push_back(cost);
						//logresBest = logres;
						lPM1b.push_back(dPM10);
						lPM2b.push_back(dPM20);
						lPM3b.push_back(dPM30);
						lFPMb.push_back(dFPM0);
						lXTb.push_back(dXT0);
						lCb.push_back(dC0);
					}
					INFEASS = true;
				}
				delete[] change1;
				//delete []endC1;
				delete[] change2;
				//delete []endC2;
				delete[] change3;
				//delete []endC3;
				delete[] change4;
				//delete []endC4;
				delete[] change5;
				//delete []endC5;
				delete[] change6;
			}
			cost_c = INT32_MAX;
			b_c_i = -1;
			MM = Costb.size();
			run += MM;
			for (int f = 0; f < MM; f++) {
				//cout << Costb[f] <<"  index:" << f << endl;
				if (cost_c > Costb[f]) {
					cost_c = Costb[f];
					b_c_i = f;
				}
			}
			//cout << "MM: " << MM << endl;
			MM = lPM1b[b_c_i].size();

			//if (run % 10 == 0) cout << best_cost << " Ts: " + to_string(Ts) << " run: " << run + 1 << endl;
			dE = best_cost - cost_c;
			if (best_cost > cost_c) {

				best_cost = cost_c;
				//cout << "------ Downhill " << best_cost << endl;
				//update best paramters
				lPM1.clear();
				lPM2.clear();
				lPM3.clear();
				lFPM.clear();
				lXT.clear();
				lC.clear();

				MM = lPM1b[b_c_i].size();
				for (int i = 0; i < MM; i++) {
					lPM1.push_back(lPM1b[b_c_i][i]);
					lPM2.push_back(lPM2b[b_c_i][i]);
					lPM3.push_back(lPM3b[b_c_i][i]);
					lFPM.push_back(lFPMb[b_c_i][i]);
					lXT.push_back(lXTb[b_c_i][i]);
					lC.push_back(lCb[b_c_i][i]);
				}
				if (bb_cost > best_cost) {
					bb_cost = best_cost;
					b_lPM1.clear();
					b_lPM2.clear();
					b_lPM3.clear();
					b_lFPM.clear();
					b_lXT.clear();
					b_lC.clear();
					MM = lPM1b[b_c_i].size();
					for (int i = 0; i < MM; i++) {

						b_lPM1.push_back(lPM1b[b_c_i][i]);
						b_lPM2.push_back(lPM2b[b_c_i][i]);
						b_lPM3.push_back(lPM3b[b_c_i][i]);
						b_lFPM.push_back(lFPMb[b_c_i][i]);
						b_lXT.push_back(lXTb[b_c_i][i]);
						b_lC.push_back(lCb[b_c_i][i]);
					}
				}
			}
			else if (r01(generator0) <= exp(dE / Ts)) {
				//cout << "uphill " << Costb[b_c_i] << endl;
				//update best paramters
				best_cost = cost_c;
				lPM1.clear();
				lPM2.clear();
				lPM3.clear();
				lFPM.clear();
				lXT.clear();
				lC.clear();
				MM = lPM1b[b_c_i].size();
				for (int i = 0; i < MM; i++) {
					lPM1.push_back(lPM1b[b_c_i][i]);
					lPM2.push_back(lPM2b[b_c_i][i]);
					lPM3.push_back(lPM3b[b_c_i][i]);
					lFPM.push_back(lFPMb[b_c_i][i]);
					lXT.push_back(lXTb[b_c_i][i]);
					lC.push_back(lCb[b_c_i][i]);
				}
			}
			if (run % 10000 == 0) cout << best_cost << " Ts: " + to_string(Ts) << " run: " << run + 1 << endl;
			//cout << " check\n";
			//cout << "d: -->" << dPM1.size() << endl;
			Ts = T_end + lam * log(1 + r_i);
			//cout << "T: "<< Ts <<"r_i: " << r_i<< endl;

			if (dE < 0) r_i++;
			else if (dE > 0)r_i = 0;
			//if (!(INFEAS1 || INFEAS2 || INFEAS3 || INFEAS4))logrestot += logres;
			// +++++++++++++++++++++++++++++++++++++++++++++++++++++++++++ HEURISTIC ++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
			//for (it = 0; it < nIt; it++) {
			//}
		}
		int  temp, xt;
		double pm1 = 1, pm2 = 1, pm3 = 1, fpm = 1;
		double b_next;
		vector<int> route;
		//double dE;
		vector<double> timetable;
		vector<double> tempt;
		double timewindow, timewindow2;
		//double Arr;

		int trips[B]; //keeps track of which trip each bus is at needs to be updated
		int best_stop;
		double freqN[N];
		double newfreqN[N];

		double bd[B];

		int yk[R][3];

		double startopt;

		int countp, p1, p2, cp1, cp2;
		bool in, in2;
		double threshold, tt;

		double l_arr, l_dep;

		int b_it = 0;

		double minF = INT16_MAX, min_al = INT16_MAX, min_ae = INT16_MAX, min_dl = INT16_MAX, min_de = INT16_MAX;
		int nS, dst;
		double diffa1, diffa2, diffd1, diffd2, diffF;
		double difBD, maxFW, maxRW;//max time to arrive or depart earlier
		//add addtional passenger with DAT
		//*
		double extra = 0, b_extra = 0;
		for (p = 0; p < R; p++) {
			yk[p][0] = -1;
			yk[p][1] = -1;
			yk[p][2] = -1;

		}
		int indexpt[R1];
		int indexpt2[R2];
		double temparrivals[R1];
		double tempdept[R2];
		for (p = 0; p < R1; p++) {
			indexpt[p] = p;
			temparrivals[p] = arrivals[p];
		}

		for (p = 0; p < R2; p++) {
			indexpt2[p] = p;
			tempdept[p] = departures[p];
		}
		quickSort(indexpt, temparrivals, 0, R1 - 1);
		quickSort(indexpt2, tempdept, 0, R2 - 1);
		best_stop = 0;
		startopt = tempdept[0] - 1000;
		threshold = 0, tt = 0;

		double cost = 0, b_cost = 0;
		//routing: [bus, trip,stops]
		vector<vector<vector<int>>> xk(B);
		route.clear();

		//D: [bus, trip,departure time]
		vector<vector<vector<double>>> Dk(B);
		timetable.clear();
		tempt.clear();
		int SL = lPM1.size();

		l_arr = arrivals[indexpt[0]], l_dep = departures[indexpt2[0]] + short_route * 0.75;
		//start_time = clock();
		//************************************Initial Solution***************************************
		for (i = 0; i < N; i++) {
			freqN[i] = -INT16_MAX;
		}
		countp = p1 = p2 = 0;
		for (b = 0; b < B; b++) {
			trips[b] = 0;
			bd[b] = startopt;//start of optimization
		}
		timetable.push_back(0);

		p1 = p2 = countp = 0;
		b = 0;
		b_next = 0.0;
		//cout << " END of PH: " << int(TS + startopt) / 60 << endl;
		b_it = -1;
		while (timetable.back() < TS + startopt) {//until TS is over+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++	
			b_it++;
			//if (nRUN > 1) {
			if (b_it < b_lPM1.size()) {
				pm1 = b_lPM1[b_it];
				pm2 = b_lPM2[b_it];
				pm3 = b_lPM3[b_it];
				fpm = b_lFPM[b_it];
				xt = int(b_lXT[b_it] * OGxt);
				C = max(5, int(b_lC[b_it] * C_OG));
			}
			else {
				//break;
				xt = OGxt;
			}
			//cout << timetable.back() / 60 << " < " << (TS + startopt) / 60 << endl;
			//}
			//xt = OGxt;
			//if (infeastemp > 100) fpm = 1;
			//cout << " xt: " << xt / 60 << endl;
			//determine which bus is available first
			b = iMin(bd, B);
			b_next = iMin2(bd, B);
			//cout << "++++++ || Bus: " << b << " bd: " << int(bd[b] / 60) << " trip: " << trips[b] << " || ++++++\n";
			//for (i = 0; i < B; i++) cout << int(bd[i] / 60) << " ";
			//cout << endl;
			//cout << "bd1: " << int(bd[b]/60) << " bd2: " << int(b_next/60) << endl;
			//Make route only with mandatory stops, ASAP
			route.clear();
			timetable.clear();
			tempt.clear();
			tt = bd[b];
			timetable.push_back(tt);
			route.push_back(0);
			for (i = 1; i < N; i++) {
				route.push_back(i);
				tt += traveltimes[i - 1][i];
				timetable.push_back(tt);
			}
			//Assignement: First look at R1
			timewindow = timewindow2 = INT32_MAX;
			cp1 = cp2 = 0;

			//++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++ CASE I ++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
			if (l_arr < l_dep && p1 < R1 || p2 == R2) {
				//cout << "R1" << endl;
				p = 0;
				while (cp1 + cp2 < C && countp < R && p < R1 && (int)(arrivals[indexpt[p]] - timewindow) <= d_ae + d_al) {
					//cout << " check start\n";
					//cout << cp1 + cp2 << "<" << C << "  " << countp << "<" << R << "  " << (int)(arrivals[indexpt[p]] - timewindow) << "< " << d_ae + d_al << endl;
					if (ttraveltimep[indexpt[p]][tclosestPS[indexpt[p]][0]] > dw) {
						std::cout << " Infeasible solution, for walking times " << endl;
						exit(0);
					}
					if (temparrivals[indexpt[p]] == -1) {
						//cout << "passenger already in solution \n";
						//cout << "p: " << p << " indexpt: " << indexpt[p] << endl;
						p++;
						if (p > R1)break;
						continue;//if this is already in the solution, continue to next passenger
					}
					if (cp1 == 0)timewindow = arrivals[indexpt[p]];

					//Assign bus stop to passenger
					//choose best stop and update route
					if (cp1 == 0) {
						//cout << " check 2 \n";
						best_stop = bestStop(timetable, route, traveltimes, ttraveltimep, tclosestPS, indexpt[p], N,
							dw, best_route, M, S, bd[b], freqN, xt, best_stop, arrivals[indexpt[p]],
							arrivals[indexpt[p]], d_ae, d_al, yk, arrivals, R1, R2, b, trips[b], b_next, fpm);
						//cout << " check stop 0\n";
						if (best_stop == -1) {
							//cout << "passenger " << indexpt[p] << " --> Skip this one, not good for bus stop\n";
							p++;
							//cout << "Next passenger " << p1 << endl;
							if (p > R1)break;
							continue;
						}
						//cout << " --- FIRST passenger added : p= " << indexpt[p] << "\n";
						//Assign bus to passenger
						yk[indexpt[p]][0] = b;
						yk[indexpt[p]][1] = trips[b];
						yk[indexpt[p]][2] = best_stop;
						temparrivals[indexpt[p]] = -1;
						tempt.push_back(arrivals[indexpt[p]]);
						p++;
						p1++;
						cp1++;
						countp++;
						if (p > R1)break;
					}
					else {
						temp = bestStop(timetable, route, traveltimes, ttraveltimep, tclosestPS, indexpt[p], N, dw,
							best_route, M, S, bd[b], freqN, xt, best_stop, tempt.back(),
							arrivals[indexpt[p]], d_ae, d_al, yk, arrivals, R1, R2, b, trips[b], b_next, fpm);
						//cout << " check stop n: " << temp <<"\n";
						if (temp == -1) {
							//cout << "passenger " << indexpt[p] << " --> Skip this one, not good for bus stop\n";
							p++;//next stop of a passenger with later edt is before a previous stop with a passenger with earlier edt, not possible OR edts are too different
							if (p > R1)break;
							continue;
						}
						else {
							// time to go from prev to next stop on a full route 
							best_stop = temp;
							//Assign bus to passenger
							yk[indexpt[p]][0] = b;
							yk[indexpt[p]][1] = trips[b];
							yk[indexpt[p]][2] = best_stop;
							temparrivals[indexpt[p]] = -1;
							tempt.push_back(arrivals[indexpt[p]]);
							p++;
							p1++;
							cp1++;
							countp++;
							if (p > R1)break;
							//cout << "-- NEXT passenger added : p= " << indexpt[p] << "\n";
							//cout << " indexpt: " << indexpt[p] << " arrivals p: " << arrivals[indexpt[p]] <<  endl;
							//cout << " p: " << p << " indexpt: " << indexpt[p] << endl;
						}
					}
				}
				//cout << " check end\n";
				//cout << "timetable:" << timetable.size() << " route: " << route.size() << " Passengers added: " << tempt.size() << endl;
			}
			// //++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++ CASE II ++++++++++++++++++++++++++++++++++++++++++++ Now look at R2
			//*
			if ((l_arr >= l_dep && p2 < R2) || (cp1 == 0 && p2 < R2)) {
				//cout << "R2" << endl;
				//cout << timetable.size() << endl;
				timetable.clear();
				tt = bd[b];
				timetable.push_back(tt);
				for (i = 1; i < N; i++) {
					tt += traveltimes[i - 1][i];
					timetable.push_back(tt);
				}

				p = 0;
				while (cp2 < C && countp < R && p < R2) {
					//cout << p2 << "<" << C << "  " << countp << "<" << R << " " << p << "< " << R2 << endl;
					//cout << "passenger " << indexpt2[p] + R1 << "  ------------- \n";
					if (ttraveltimep[R1 + indexpt2[p]][tclosestPS[R1 + indexpt2[p]][0]] > dw) {
						std::cout << " Infeasible solution, for walking times " << endl;
						exit(0);
					}
					if (tempdept[indexpt2[p]] == -1) {
						//cout << "passenger already in solution \n";
						p++;
						continue;//if this is already in the solution, continue to next passenger
					}
					t = 0;
					for (i = 0; i < timetable.size(); i++) {
						if (route[i] < N) {
							newfreqN[t] = timetable[i];
							t++;
						}
					}
					if (cp2 == 0) {
						best_stop = bestStop2(timetable, route, traveltimes, ttraveltimep, tclosestPS, indexpt2[p] + R1, N,
							dw, best_route, M, S, bd[b], freqN, newfreqN, xt, best_stop, departures[indexpt2[p]],
							departures[indexpt2[p]], d_de, d_dl, yk, departures, R1, R2, b, trips[b], b_next, fpm);
						if (best_stop == -1) {
							//cout << "passenger " << indexpt2[p] + R1 << " --> Skip this one, not good for bus stop\n";
							p++;
							//cout << "Next passenger " << p1 << endl;
							continue;
						}
						//cout << " --- FIRST passenger added : p= " << indexpt2[p] + R1 << "\n";
						//Assign bus to passenger
						yk[R1 + indexpt2[p]][0] = b;
						yk[R1 + indexpt2[p]][1] = trips[b];
						//Assign bus stop to passenger
						//choose best stop and update route
						//cout << "  Best stop: " << best_stop << endl;
						//cout << "Passenger: " << R1 + indexpt2[p] << " edt: " << int(departures[indexpt2[p]] / 60) << " stop: " << best_stop << endl;
						tempt.push_back(departures[indexpt2[p]]);
						tempdept[indexpt2[p]] = -1;
						yk[R1 + indexpt2[p]][2] = best_stop;
						cp2++;
						p2++;
						countp++;
						p++;
					}
					else {
						temp = bestStop2(timetable, route, traveltimes, ttraveltimep, tclosestPS, indexpt2[p] + R1, N, dw,
							best_route, M, S, bd[b], freqN, newfreqN, xt, best_stop, tempt.back(),
							departures[indexpt2[p]], d_de, d_dl, yk, departures, R1, R2, b, trips[b], b_next, fpm);

						if (temp == -1) {
							//cout << "Passenger: " << R1 + indexpt2[p] << " edt: "<< int(departures[indexpt2[p]] / 60) << "Skip this one, not good for bus stop\n";
							p++;//next stop of a passenger with later edt is before a previous stop with a passenger with earlier edt, not possible OR edts are too different
							continue;
						}
						else {
							// time to go from prev to next stop on a full route 
							best_stop = temp;
							//Assign bus to passenger
							yk[R1 + indexpt2[p]][0] = b;
							yk[R1 + indexpt2[p]][1] = trips[b];
							//Assign bus stop to passenger
							yk[R1 + indexpt2[p]][2] = best_stop;
							//cout << "Passenger: " << R1 + indexpt2[p] << " edt: " << int(departures[indexpt2[p]] / 60) << " stop: " << best_stop << endl;
							tempt.push_back(departures[indexpt2[p]]);
							tempdept[indexpt2[p]] = -1;
							cp2++;
							p2++;
							countp++;
							p++;
							//cout << "-- NEXT passenger added : p= " << indexpt2[p] + R1 << "\n";
						}
					}
				}

				if (p1 < R1 && l_arr >= l_dep && tempt.size() == 0) {
					//++++++++++++++++++++++++++++++++ CASE I ++++++++++++++++++++++++++++++++++++++++++++
					//cout << "R1 again" << endl;
					p = 0;
					while (cp1 + cp2 < C && countp < R && p < R1 && (int)(arrivals[indexpt[p]] - timewindow) <= d_ae + d_al) {
						//cout << cp1 + cp2 << "<" << C << "  " << countp << "<" << R << "  " << (int)(arrivals[indexpt[p]] - timewindow) << "< " << d_ae + d_al << endl;
						if (ttraveltimep[indexpt[p]][tclosestPS[indexpt[p]][0]] > dw) {
							std::cout << " Infeasible solution, for walking times " << endl;
							exit(0);
						}
						if (temparrivals[indexpt[p]] == -1) {
							//cout << "passenger already in solution \n";
							//cout << "p: " << p << " indexpt: " << indexpt[p] << endl;
							p++;
							if (p > R1)break;
							continue;//if this is already in the solution, continue to next passenger
						}
						if (cp1 == 0)timewindow = arrivals[indexpt[p]];
						//Assign bus stop to passenger
						//choose best stop and update route
						if (cp1 == 0) {
							best_stop = bestStop(timetable, route, traveltimes, ttraveltimep, tclosestPS, indexpt[p], N,
								dw, best_route, M, S, bd[b], freqN, xt, best_stop, arrivals[indexpt[p]],
								arrivals[indexpt[p]], d_ae, d_al, yk, arrivals, R1, R2, b, trips[b], b_next, fpm);
							//cout << " check\n";
							if (best_stop == -1) {
								//cout << "passenger " << indexpt[p]  << " --> Skip this one, not good for bus stop\n";
								p++;
								//cout << "Next passenger " << p1 << endl;
								if (p > R1)break;
								continue;
							}
							//cout << " --- FIRST passenger added\n";
							//Assign bus to passenger
							yk[indexpt[p]][0] = b;
							yk[indexpt[p]][1] = trips[b];
							yk[indexpt[p]][2] = best_stop;
							temparrivals[indexpt[p]] = -1;
							tempt.push_back(arrivals[indexpt[p]]);
							p++;
							p1++;
							cp1++;
							countp++;
							if (p > R1)break;
							//cout << " --- FIRST passenger added : p= " << indexpt[p] << "\n";
						}
						else {
							temp = bestStop(timetable, route, traveltimes, ttraveltimep, tclosestPS, indexpt[p], N, dw,
								best_route, M, S, bd[b], freqN, xt, best_stop, tempt.back(),
								arrivals[indexpt[p]], d_ae, d_al, yk, arrivals, R1, R2, b, trips[b], b_next, fpm);
							//cout << " check\n";
							if (temp == -1) {
								//cout << "passenger " << indexpt[p] << " --> Skip this one, not good for bus stop\n";
								p++;//next stop of a passenger with later edt is before a previous stop with a passenger with earlier edt, not possible OR edts are too different
								if (p > R1)break;
								continue;

							}
							else {
								// time to go from prev to next stop on a full route 
								best_stop = temp;
								//Assign bus to passenger
								yk[indexpt[p]][0] = b;
								yk[indexpt[p]][1] = trips[b];
								yk[indexpt[p]][2] = best_stop;
								temparrivals[indexpt[p]] = -1;
								tempt.push_back(arrivals[indexpt[p]]);
								p++;
								p1++;
								cp1++;
								countp++;
								if (p > R1)break;
								//cout << "-- NEXT passenger added : p= " << indexpt[p] << "\n";
							}
						}

					}

				}
				//cout << "timetable:" << timetable.size() << " route: " << route.size() << " Passengers added: " << tempt.size() << endl;
			}
			//*/
			//cout << "freq at 0: " << freqN[0] / 60 << endl;
			
			if (cp1 + cp2 == 0) {
				timetable.clear();
				//timetable.push_back(max(freqN[0] + xt, bd[b]));
				if (b_it < b_lPM1.size()) timetable.push_back(max(freqN[0] + xt, bd[b]));
				else timetable.push_back(max(bd[b], freqN[0] + xt/2));
				tt = -1;
				for (i = 1; i < N; i++) {
					timetable.push_back(timetable[i - 1] + traveltimes[i - 1][i]);
					if (timetable[i] - freqN[i] > tt)tt = timetable[i] - freqN[i];
				}
				if (tt > xt) {
					for (i = 0; i < N; i++) {
						timetable[i] -= (tt - xt);
					}
				}
				/*
				if (b_it >= b_lPM1.size()) {
					cout << "nobody onboard bus " << b << " on trip " << trips[b] << " bd: " << round(bd[b] / 60 * 100) / 100 << " tt: " << tt / 60 << " xt " << xt / 60 << "\n";
					for (int ii = 0; ii < timetable.size(); ii++) {
						cout << round(timetable[ii] / 60) << "\t";
					}
					cout << endl<< " freqN:\n";
					for (int ii = 0; ii < N; ii++) {
						cout << round(freqN[ii] / 60) << "\t";
					}
					cout << endl;
				}
				//*/
			}
			else if (cp1 + cp2 != 0 ) {
				//add addtional passenger with DAT
				minF = INT16_MAX, min_al = INT16_MAX, min_ae = INT16_MAX, min_dl = INT16_MAX, min_de = INT16_MAX;
				nS = timetable.size() - 1;

				for (p = 0; p < R1; p++) {
					if (yk[p][0] == b && yk[p][1] == trips[b]) {
						//cout << "passenger " << p  << ": dat=" << int(arrivals[p] / 60) << " arr=" << int(timetable[nS] / 60) << endl;
						diffa1 = d_ae - (arrivals[p] - timetable[nS]);
						diffa2 = d_al - (timetable[nS] - arrivals[p]);
						if (arrivals[p] >= timetable[nS] && min_ae > diffa1) {
							min_ae = diffa1;
						}
						if (arrivals[p] <= timetable[nS] && min_al > diffa2) {
							min_al = diffa2;
						}
					}
				}

				dst = -1;
				for (p = 0; p < R2; p++) {
					dst = -1;
					for (i = 0; i < route.size(); i++) {
						if (yk[p + R1][0] == b && yk[p + R1][1] == trips[b] && yk[p + R1][2] == route[i]) {
							dst = i;
							break;
						}
					}
					if (dst != -1) {
						//cout << "passenger " << p +R1 << ": edt=" << int(departures[p] / 60) << " dept=" << int(timetable[dst] / 60) << " at stop: " <<route[dst] << endl;
						diffd1 = d_de - (departures[p] - timetable[dst]);
						diffd2 = d_dl - (timetable[dst] - departures[p]);
						if (departures[p] >= timetable[dst] && min_de > diffd1) {
							min_de = diffd1;
						}
						if (departures[p] <= timetable[dst] && min_dl > diffd2) {
							min_dl = diffd2;
						}
					}
				}
				if (freqN[0] != -INT16_MAX) {
					for (i = 0; i < timetable.size(); i++) {
						if (route[i] < N && freqN[route[i]] < timetable[i]) {
							diffF = xt - (timetable[i] - freqN[route[i]]);
							if (minF > diffF) minF = diffF;
						}
					}
				}

				//cout << " min diff al: " << int(min_al/60) << " min diff dl: " << int(min_dl/60) << " min diff Freq: " << int(minF / 60) << endl;
				//cout << " min diff ae: " << int(min_ae/60) << " min diff de: " << int(min_de/60) << endl;
				difBD = timetable[0] - bd[b];
				maxFW = min(min(min_al, min_dl), minF) * pm1; // max time to depart later 
				maxRW = min(min(min_ae, min_de), difBD) * pm2;//max time to arrive or depart earlier
				//add addtional passenger with DAT
				//*
				extra = 0;
				//cout << "Max time FW: " << int(maxFW/60) << " max time RW: " << int(maxRW/60) << "\nStart R1\n";
				//cout << "R1: " << cp1 << " R2: " << cp2 << endl;

				for (p = 0; p < R1; p++) {
					if (temparrivals[p] != -1) {
						//cout << "for p_" << p << " dat: "<<arrivals[p]/60<< " arrval: " << timetable.back()/60<<endl;
						i = 0;
						b_cost = INT32_MAX;
						s = -1;
						if (arrivals[p] >= timetable.back()) {
							if (arrivals[p] - timetable.back() <= d_ae) extra = 0;
							else extra = (arrivals[p] - timetable.back()) - d_ae;
						}
						else {
							if (timetable.back() - arrivals[p] <= d_al) extra = 0;
							else extra = d_al - (timetable.back() - arrivals[p]);
						}
						while (ttraveltimep[p][tclosestPS[p][i]] < dw * pm3) {
							//cout << "try stop: " << tclosestPS[p][i] << endl;
							for (t = 0; t < route.size() - 1; t++) {
								in = false;
								if (tclosestPS[p][i] == route[t]) {
									in = true;
									break;
								}
							}
							in2 = false;
							if (in && arrivals[p] - timetable.back() - maxFW <= d_ae && timetable.back() - maxRW - arrivals[p] <= d_al) {
								in2 = true;
							}

							if (in2 && cp1 + cp2 < C) {
								//cout << " ++++++++++++ passenger " << p << " (dat=" << int(arrivals[p] / 60) << ") stop " << tclosestPS[p][i] << " (tt=" << int(timetable.back() / 60) << ") possible extra: " << int(extra / 60) << endl;
								cost = c2 * ttraveltimep[p][tclosestPS[p][i]] + c3 * abs(timetable.back() - arrivals[p]) + c3 * (cp1 + cp2) * (abs(extra));
								if (b_cost > cost) {
									b_cost = cost;
									s = route[t];
								}
							}
							i++;
						}
						if (b_cost != INT32_MAX) {
							//cout << "Added DAT \t";
							//cout << "Passenger: " << p << " arr: " << arrivals[p] / 60 << endl;
							yk[p][0] = b;
							yk[p][1] = trips[b];
							yk[p][2] = s;
							//cout << "Passenger: " << R1 + p << endl;
							temparrivals[p] = -1;//indicate this passenger is onboard already
							p1++;
							cp1++;
							countp++;
							//break;

							//update timetable
							//cout << " move tt with " << int(extra / 60) << endl;
							for (l = 0; l < timetable.size(); l++) {
								timetable[l] += extra;
							}
							//update intermediary parameters
							minF = INT16_MAX, min_al = INT16_MAX, min_ae = INT16_MAX, min_dl = INT16_MAX, min_de = INT16_MAX;
							nS = timetable.size() - 1;
							for (int p11 = 0; p11 < R1; p11++) {
								if (yk[p11][0] == b && yk[p11][1] == trips[b]) {
									diffa1 = d_ae - (arrivals[p11] - timetable[nS]);
									diffa2 = d_al - (timetable[nS] - arrivals[p11]);
									if (arrivals[p11] >= timetable[nS] && min_ae > diffa1) {
										min_ae = diffa1;
									}
									if (arrivals[p11] <= timetable[nS] && min_al > diffa2) {
										min_al = diffa2;
									}
								}
							}
							for (int p22 = 0; p22 < R2; p22++) {
								dst = -1;
								for (l = 0; l < route.size(); l++) {
									if (yk[p22 + R1][0] == b && yk[p22 + R1][1] == trips[b] && yk[p22 + R1][2] == route[l]) {
										dst = l;
										break;
									}
								}
								if (dst != -1) {
									diffd1 = d_de - (departures[p22] - timetable[dst]);
									diffd2 = d_dl - (timetable[dst] - departures[p22]);
									if (departures[p22] >= timetable[dst] && min_de > diffd1) {
										min_de = diffd1;
									}
									if (departures[p22] <= timetable[dst] && min_dl > diffd2) {
										min_dl = diffd2;
									}
								}
							}
							if (freqN[0] != -INT16_MAX) {
								for (l = 0; l < timetable.size(); l++) {
									if (route[l] < N && freqN[route[l]] < timetable[l]) {
										diffF = xt - (timetable[l] - freqN[route[l]]);
										if (minF > diffF) minF = diffF;
									}
								}
							}
							//cout << " min diff al: " << int(min_al/60) << " min diff dl: " << int(min_dl/60) << endl;
							//cout << " min diff ae: " << int(min_ae/60) << " min diff de: " << int(min_de/60) << " min diff Freq: " << int(minF/60) << endl;
							difBD = timetable[0] - bd[b];
							maxFW = min(min(min_al, min_dl), minF) * pm1; // max time to depart later 
							maxRW = min(min(min_ae, min_de), difBD) * pm2;//max time to arrive or depart earlier
							//cout << "  Added passenger " << p << " --> NEW Max time FW: " << int(maxFW/60) << " max time RW: " << int(maxRW/60) << endl;

						}
					}
				}

				//add addtional passenger with EDT
				///*
				//cout << "Start R2\nNEW Max time FW : " << int(maxFW / 60) << " max time RW : " << int(maxRW / 60) << endl;
				b_extra = 0;
				for (p = 0; p < R2; p++) {
					if (tempdept[p] != -1) {
						//cout << "try p_" << p+R1 << " dept: "<<departures[p]/60<<endl;
						i = 0;
						b_cost = INT32_MAX;
						s = -1;
						while (ttraveltimep[R1 + p][tclosestPS[p + R1][i]] < dw * pm3) {
							//if (R1 + p == 36)cout << tclosestPS[p + R1][i] << endl;
							for (t = 0; t < route.size() - 1; t++) {
								in = false;
								if (tclosestPS[p + R1][i] == route[t]) {
									//cout << "bus stop " << tclosestPS[p + R1][i] << " position " << t << endl;
									in = true;
									break;
								}
							}
							in2 = false;
							if (in && departures[p] - timetable[t] - maxFW <= d_de && timetable[t] - maxRW - departures[p] <= d_dl) {
								in2 = true;
							}
							//if (in2) cout << "bus stop " << tclosestPS[p + R1][i] << " position " << t << " length of tt: " << timetable.size() << endl;

							if (in2 && cp1 + cp2 < C) {
								//cout << "bus stop " << tclosestPS[p + R1][i] << " position " << t << " length of tt: " << timetable.size() << endl;
								if (departures[p] >= timetable[t]) {
									if (departures[p] - timetable[t] <= d_de)extra = 0;
									else extra = (departures[p] - timetable[t]) - d_de;
								}
								else {
									if (timetable[t] - departures[p] <= d_dl)extra = 0;
									else extra = d_dl - (timetable[t] - departures[p]);
								}
								//cout << "walking time " << ttraveltimep[R1 + p][tclosestPS[p + R1][i]] / 60 << endl;
								//cout << " ++++++++++++ passenger " << p+R1 << " (edt="<< int(departures[p]/60) << ") stop " << tclosestPS[p + R1][i] <<" (tt="<< int(timetable[t]/60) << ") possible extra: " << int(extra / 60)<< endl;
								cost = c2 * ttraveltimep[R1 + p][tclosestPS[p + R1][i]] + c3 * abs(timetable[t] - departures[p]) + c3 * (cp1 + cp2) * (abs(extra));
								if (b_cost > cost) {
									b_cost = cost;
									s = route[t];
									b_extra = extra;
								}
							}
							i++;
						}
						if (b_cost != INT32_MAX) {
							//cout << "Added EDT \t";
							//cout << "Passenger: " << R1 + p << " dept: " << departures[p] / 60 << endl;
							yk[p + R1][0] = b;
							yk[p + R1][1] = trips[b];
							yk[p + R1][2] = s;
							//cout << "Passenger: " << R1 + p << endl;
							tempdept[p] = -1;//indicate this passenger is onboard already
							p2++;
							cp2++;
							countp++;
							//break;
							//update timetable
							//cout << " move tt with " << int(b_extra / 60) << endl;
							for (l = 0; l < timetable.size(); l++) {
								timetable[l] += b_extra;
							}
							//update intermediary parameters
							minF = INT16_MAX, min_al = INT16_MAX, min_ae = INT16_MAX, min_dl = INT16_MAX, min_de = INT16_MAX;
							nS = timetable.size() - 1;
							for (int p11 = 0; p11 < R1; p11++) {
								if (yk[p11][0] == b && yk[p11][1] == trips[b]) {
									diffa1 = d_ae - (arrivals[p11] - timetable[nS]);
									diffa2 = d_al - (timetable[nS] - arrivals[p11]);
									if (arrivals[p11] >= timetable[nS] && min_ae > diffa1) {
										min_ae = diffa1;
									}
									if (arrivals[p11] <= timetable[nS] && min_al > diffa2) {
										min_al = diffa2;
									}
								}
							}
							for (int p22 = 0; p22 < R2; p22++) {
								dst = -1;
								for (l = 0; l < route.size(); l++) {
									if (yk[p22 + R1][0] == b && yk[p22 + R1][1] == trips[b] && yk[p22 + R1][2] == route[l]) {
										dst = l;
										break;
									}
								}
								if (dst != -1) {
									diffd1 = d_de - (departures[p22] - timetable[dst]);
									diffd2 = d_dl - (timetable[dst] - departures[p22]);
									if (departures[p22] >= timetable[dst] && min_de > diffd1) {
										min_de = diffd1;
									}
									if (departures[p22] <= timetable[dst] && min_dl > diffd2) {
										min_dl = diffd2;
									}
								}
							}
							if (freqN[0] != -INT16_MAX) {
								for (l = 0; l < timetable.size(); l++) {
									if (route[l] < N && freqN[route[l]] < timetable[l]) {
										diffF = xt - (timetable[l] - freqN[route[l]]);
										if (minF > diffF) minF = diffF;
									}
								}
							}

							difBD = timetable[0] - bd[b];
							maxFW = min(min(min_al, min_dl), minF) * pm1; // max time to depart later 
							maxRW = min(min(min_ae, min_de), difBD) * pm2;//max time to arrive or depart earlier
							//cout << "    min diff al: " << int(min_al/60) << " min diff dl: " << int(min_dl/60) << endl;
							//cout << "    min diff ae: " << int(min_ae/60) << " min diff de: " << int(min_de/60) << " min diff Freq: " << int(minF/60) << endl;
							//cout << "  Added passenger " << p + R1 << " --> NEW Max time FW: " << int(maxFW / 60) << " max time RW: " << int(maxRW / 60) << endl;
						}
					}
				}
				//cout << "AFTER --> R1: " << cp1 << " R2: " << cp2 << endl;
				//*/

			}
			//cout << "R1+R2 done  Nroute: " << route.size() << " N_timetable: " << timetable.size() << endl;
			//update trip of bus b
			trips[b]++;
			//update bd of bus b
			bd[b] = timetable.back() + short_route;
			//update varaibles
			xk[b].push_back(route);
			Dk[b].push_back(timetable);
			//update frequency at mandatory 
			//cout << "xt constraints: \n";
			//cout << " ******* TIMETABLE: ";
			for (i = 0; i < timetable.size(); i++) {
				if (route[i] < N && (freqN[route[i]] < timetable[i] || freqN[route[i]] - timetable[i] > xt)) {
					//if(timetable[i] - freqN[route[i]]+1>xt) cout << int((timetable[i]- freqN[route[i]])) / 60 << " (s: "<< route[i]<< ") ";
					freqN[route[i]] = timetable[i];
					t++;
				}
				//cout << int(timetable[i] / 60) << " ";
			}
			if (p2 < R2) {
				for (p = 0; p < R2; p++) {
					if (tempdept[indexpt2[p]] != -1) {
						l_dep = departures[indexpt2[p]] + 0.75 * short_route;
						break;
					}
				}
			}
			if (p1 < R1) {
				for (p = 0; p < R1; p++) {
					if (temparrivals[indexpt[p]] != -1) {
						l_arr = arrivals[indexpt[p]];
						break;
					}
				}
			}
		}

		double telapsed_time = (double)(clock() - tstart_time) / CLK_TCK;

		for (int b = 0; b < B; b++) {
			vector<vector<int>> xsol1(xk[b].size());
			vector<vector<double>> Dsol1(Dk[b].size());
			for (int t = 0; t < xk[b].size(); t++) {
				vector<int> xsol2(xk[b][t].size());
				vector<double> Dsol2(Dk[b][t].size());
				for (int i = 0; i < xk[b][t].size(); i++) {
					xsol2[i] = xk[b][t][i];
					Dsol2[i] = Dk[b][t][i];
				}
				xsol1[t] = xsol2;
				Dsol1[t] = Dsol2;
			}
			b_Dsol[b] = Dsol1;
			b_xsol[b] = xsol1;
		}
		for (int p = 0; p < R; p++) {
			b_ysol[p][0] = yk[p][0];
			b_ysol[p][1] = yk[p][1];
			b_ysol[p][2] = yk[p][2];
		}
		
		cout << " Number of good feasible attempts: " << run << endl;
		cout << "\tcost: " << best_cost << "\truntime: " << telapsed_time << endl;

		ofstream xsol_p("data/output/xsol_" + to_string(instance) + ".txt");
		ofstream ysol_p("data/output/ysol_" + to_string(instance) + ".txt");
		ofstream dsol_p("data/output/dsol_" + to_string(instance) + ".txt");
		for (int i = 0; i < b_xsol.size(); i++) {
			xsol_p << "BUS " << i << endl;
			dsol_p << "BUS " << i << endl;
			for (int j = 0; j < b_xsol[i].size(); j++) {
				for (int k = 0; k < b_xsol[i][j].size(); k++) {
					xsol_p << b_xsol[i][j][k] << "\t";
				}
				xsol_p << endl;
				for (int k = 0; k < b_xsol[i][j].size(); k++) {
					dsol_p << b_Dsol[i][j][k] << "\t";
				}
				dsol_p << endl;
			}
		}
		//Objective function value
		ysol_p << "Bus" << "\t" << "Trip" << "\t" << "stop" << endl;
		for (int p = 0; p < R; p++) {
			int b = b_ysol[p][0];
			int t = b_ysol[p][1];
			int s = b_ysol[p][2];
			ysol_p << b << "\t" << t << "\t" << s << endl;
		}

		xsol_p.close();
		ysol_p.close();
		dsol_p.close();

		for (i = 0; i < R; i++) {
			delete []ttraveltimep[i];
			delete []tclosestPS[i];
		}
		delete []ttraveltimep;
		delete []tclosestPS;
	}
		else {
			cout << "READ SO\n";
			ifstream xsol_p("data/output/xsol_" + to_string(instance) + ".txt");
			ifstream ysol_p("data/output/sol_" + to_string(instance) + ".txt");
			ifstream dsol_p("data/output/dsol_" + to_string(instance) + ".txt");

			string line;
			if (dsol_p.is_open()) { // make sure the file opening was successful
				b = -1;
				while (getline(dsol_p, line)) {
					if (line.find("BUS") != std::string::npos) {
						b++;
					}
					else {
						vector<double> vd;
						double tempd;
						stringstream ss(line); // convert string into a stream
						while (ss >> tempd) {     // convert each word on the stream into an int
							vd.push_back(tempd); // store it in the vector
						}
						b_Dsol[b].push_back(vd);
					}
				}
			}

			if (xsol_p.is_open()) { // make sure the file opening was successful
				b = -1;
				while (getline(xsol_p, line)) {
					if (line.find("BUS") != std::string::npos) {
						b++;
					}
					else {
						vector<int> vx;
						int tempx;
						stringstream ss(line); // convert string into a stream
						while (ss >> tempx) {     // convert each word on the stream into an int
							vx.push_back(tempx); // store it in the vector
						}
						b_xsol[b].push_back(vx);
					}
				}
			}
			if (ysol_p.is_open()) { // make sure the file opening was successful
				p = -1;
				while (getline(ysol_p, line)) {
					if (line.find("Bus") != std::string::npos) {
						p++;
					}
					else {
						//cout << p << endl;
						stringstream ss(line); // convert string into a stream
						ss >> b_ysol[p][0] >> b_ysol[p][1] >> b_ysol[p][2];
						p++;
					}
				}
			}
		}
	}
	double pickup[OG_R];
	int pickupstops[OG_R];
	vector<vector<int>> ytemp(OG_R);
	for (p = R1; p < OG_R; p++) {
		vector<int> yttemp(3);
		yttemp[0] = b_ysol[p][0];
		yttemp[1] = b_ysol[p][1];
		yttemp[2] = b_ysol[p][2];
		ytemp[p] = yttemp;
	}
	for (p = R1; p < R; p++) {
		b_ysol[p + OG_R1 - R1][0] = ytemp[p][0];
		b_ysol[p + OG_R1 - R1][1] = ytemp[p][1];
		b_ysol[p + OG_R1 - R1][2] = ytemp[p][2];
	}
	for (p = R1; p < OG_R1; p++) {
		b_ysol[p][0] = -1;
		b_ysol[p][1] = -1;
		b_ysol[p][2] = -1;
	}
	int NN = 0;
	for (p = 0; p < OG_R; p++) {
		if (b_ysol[p][0] != -1) {
			NN = b_xsol[b_ysol[p][0]][b_ysol[p][1]].size();
			for (i = 0; i < NN; i++) {
				if (b_xsol[b_ysol[p][0]][b_ysol[p][1]][i] == b_ysol[p][2]) break;
			}
			pickup[p] = b_Dsol[b_ysol[p][0]][b_ysol[p][1]][i];
			pickupstops[p] = b_xsol[b_ysol[p][0]][b_ysol[p][1]][i];
		}
		else {
			pickupstops[p] = -1;
			pickup[p] = -1;
		}
	}
	
	double penalty = 1.5 * c1 * short_route + c2 * dw + c3 * max(d_ae, d_al);
	//cout << " ------------------------------------------ PENALTY " << penalty << endl;
	double cost = printpluscost(b_xsol, b_Dsol, b_ysol, B, OG_R, OG_R1, N, traveltimes, traveltimep, pickup, OG_departures, OG_arrivals, c1, c2, c3, OGxt, d_ae, d_al, d_dl, d_de, dw, penalty, short_route, closestPS, d_t,true,isFEAS);

	//remove memory
	for (i = 0; i < N; i++) {
		delete[] mandatory[i];
	}
	delete[] mandatory;

	for (i = 0; i < (N - 1) * M; i++) {
		delete []optional[i];
	}
	delete[] optional;

	for (i = 0; i < OG_R; i++) {
		delete[] passengers[i];
	}
	delete[] passengers;

	// ****************************************************************************************** ADJUST PLANNING with new requests ***********************************************************************************************
	
	//assume you have the initial schedule (b_xxxsol)
	int CBus = C_OG; // current cpacity of bus b
	double lam1 = 20 * 60, lam2 = 20 * 60, lam3 = 15 * 60; // parameters 
	double mindiffarr = INT32_MAX;
	int minstoparr = 0;
	double diffarr = 0; // difference in departure from bus stop and timestamp
	double min_est = INT32_MAX;
	bool in_est = false;
	double d_est = -1;
	int s_est = -1; //estimate departure and stop of passenger 
	double d_last = -1; // departure at last stop
	int FCn; // determine departures at mand stops
	vector<vector<vector<double>>> FC(N);
	int b_stop = -1, b_s = -1, b_bus = -1, b_trip = -1, nBus = 0;
	double b_cost = INT32_MAX, c_cost = -1, cost_temp = 0;
	route.clear();
	timetable.clear();
	vector<int> b_route;
	vector<double> b_timetable;

	double temptimestamp[OG_R];
	int indextimestamp[OG_R];
	for (p = 0; p < OG_R; p++) {
		indextimestamp[p] = p;
		temptimestamp[p] = timestamps[p];
	}
	quickSort(indextimestamp, temptimestamp, 0, OG_R - 1); // sort passengers accounding to time of request, S_O passengers will be timestamp = -1

	int prevstop = 0, nextstop = 0; //stops the bus just visited and is going to visit next
	int pt = 0, nstops = 0; 

	bool currdrive = false, bestdriving = false;
	vector<int> Pa;
	//double bd[B];
	//int trips[B];
	//double freqN[N];
	endtime = minTS + TS;
	double start_time = 0, elapsed_time = 0;
	ofstream rt_p("data/output/runtimes_" + to_string(instance) + ".txt");
	ofstream obj_p("data/output/obj_" + to_string(instance) + ".txt");
	double max_runtime = 0, current_time = 0, midpoint = 0;

	//+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++ BEGIN INSERTION ALGORITHM ++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
	for (p = 0; p < OG_R; p++) {
		if (temptimestamp[p] != -1) {
			//determine state of the system at timestamp of passenger p
			current_time = max(temptimestamp[p], current_time);
			b_cost = INT32_MAX;
			c_cost = -1;
			pt = indextimestamp[p];
			s_est = closestPS[pt][0];
			if (pt < OG_R1) cout << " ++++++++++++++++++++ Passenger " << pt << " (" << p + 1 << "/" << OG_R << ") made a request at time: " << round(timestamps[pt] / 60) << " with DAT=" << round(OG_arrivals[pt] / 60) << " closest stop is " << s_est << endl;
			else cout << " ++++++++++++++++++++ Passenger " << pt << " (" << p+1 << "/" << OG_R << ") made a request at time : " << round(timestamps[pt] / 60) << " with DDT = " << round(OG_departures[pt - OG_R1] / 60) << " closest stop is " << s_est << endl;
			
			start_time = clock();
			//departures at mandatory stops
			for (i = 0; i < N; i++) {
				FC[i].clear();
				for (b = 0; b < B; b++) {
					X1 = b_xsol[b].size();
					for (t = 0; t < X1; t++) {
						X2 = b_xsol[b][t].size();
						for (l = 0; l < X2; l++) {
							if (b_xsol[b][t][l] == i)FC[i].push_back({ b_Dsol[b][t][l], float(b), float(t) });
						}
					}
				}
			}
			//Loop on the vehicles to see where the best assignment is.
			//filter vehicles based on current location: if vehicle has passed the location of passenger (with BR), if in the current planning the timetable is too far from the DAT or DDT
			for (b = 0; b < B; b++) {
				for (t = 0; t < b_Dsol[b].size(); t++) {
					currdrive = false;
					nstops = b_Dsol[b][t].size();
					//Calculate capacity
					CBus = C_OG;
					for (l = 0; l < OG_R; l++) {
						if (b_ysol[l][0] == b && b_ysol[l][1] == t) {
							CBus--;
						}
					}
					//Calculate departure time from s_est
					if (CBus >= 1) {
						in_est = false;
						min_est = INT32_MAX;
						for (j = 0; j < nstops; j++) {
							if (b_xsol[b][t][j] == s_est) {
								d_est = b_Dsol[b][t][j];
								in_est = true;
								d_last = b_Dsol[b][t][nstops - 1];
								break;
							}
							if (min_est > traveltimes[b_xsol[b][t][j]][s_est]) {
								min_est = traveltimes[b_xsol[b][t][j]][s_est];
								d_est = b_Dsol[b][t][j] + min_est;
								if (j != nstops - 1) d_last = b_Dsol[b][t][nstops - 1] + traveltimes[b_xsol[b][t][j]][s_est] + traveltimes[b_xsol[b][t][j + 1]][s_est] - traveltimes[b_xsol[b][t][j + 1]][b_xsol[b][t][j]];
								else d_last = b_Dsol[b][t][nstops - 1];
							}
						}
						//cout << "bus " << b << " trip " << t << " estimated departure " << d_est/60 << " estimated arrival " << d_last/60 << " walking " << traveltimep[pt][s_est]/60 <<  endl;
						if (pt < OG_R1) {
							//cout << "bus " << b << " trip " << t << " time departure " << int(d_est / 60) << " time arrival " << int(d_last / 60) << endl;
							if (abs(d_last - OG_arrivals[pt]) <= lam1 && traveltimep[pt][s_est] + timestamps[pt] - d_est <= lam3) {
								//cout << " FEASIBLE From bus " << b << " on trip " << t << " with estimated departure at closest stop: " << int(d_est / 60) << " and with estimated arrival at destination: " << int(d_last / 60);
								if (b_Dsol[b][t][0]< current_time && b_Dsol[b][t][nstops - 1]>current_time) {
									//determine bustops previously vsisited and next to visit
									currdrive = true;
									mindiffarr = INT16_MAX;
									for (j = 0; j < b_Dsol[b][t].size(); j++) {
										diffarr = abs(b_Dsol[b][t][j] - current_time);
										if (mindiffarr > diffarr) {
											mindiffarr = diffarr;
											minstoparr = j;
										}
									}
									if (b_Dsol[b][t][minstoparr] > current_time) {
										nextstop = b_xsol[b][t][minstoparr];
										prevstop = b_xsol[b][t][minstoparr - 1];
									}
									else {
										nextstop = b_xsol[b][t][minstoparr + 1];
										prevstop = b_xsol[b][t][minstoparr];
									}
									//cout << " is currently driving between stop " << prevstop << " and stop " << nextstop << " , current capacity is " << CBus;
									c_cost = insertstop(route, timetable, N, M, S, OG_R, OG_R1, OG_R2, OGxt, d_dl, d_de, d_ae, d_al, d_t, short_route, pickup, OG_departures, OG_arrivals, best_route,
										traveltimes, traveltimep, closestPS, dw, FC, b_xsol, b_Dsol, b_ysol, pt, b, t, current_time, true, prevstop, nextstop, max_wait, b_stop, c1, c2, c3, 0);
								}
								else {
									//cout << " is not currently driving ";
									c_cost = insertstop(route, timetable, N, M, S, OG_R, OG_R1, OG_R2, OGxt, d_dl, d_de, d_ae, d_al, d_t, short_route, pickup, OG_departures, OG_arrivals, best_route,
										traveltimes, traveltimep, closestPS, dw, FC, b_xsol, b_Dsol, b_ysol, pt, b, t, current_time, false, -1, -1, max_wait, b_stop, c1, c2, c3, 0);
								}
								//cout << endl;
							}
						}
						else {
							//cout << "bus " << b << " trip " << t << " time departure " << int(d_est / 60) << " time arrival " << int(d_last / 60) << endl;
							if (abs(d_est - OG_departures[pt - OG_R1]) <= lam2 && traveltimep[pt][s_est] + timestamps[pt] - d_est <= lam3) {
								//cout << " FEASIBLE From bus " << b << " on trip " << t << " with estimated departure at closest stop: " << int(d_est / 60) << " and with estimated arrival at destination: " << int(d_last / 60);
								if (b_Dsol[b][t][0]< current_time && b_Dsol[b][t][nstops - 1]>current_time) {
									//determine bustops previously vsisited and next to visit
									mindiffarr = INT16_MAX;
									currdrive = true;
									for (j = 0; j < b_Dsol[b][t].size(); j++) {
										diffarr = abs(b_Dsol[b][t][j] - current_time);
										if (mindiffarr > diffarr) {
											mindiffarr = diffarr;
											minstoparr = j;
										}
									}
									if (b_Dsol[b][t][minstoparr] > current_time) {
										nextstop = b_xsol[b][t][minstoparr];
										prevstop = b_xsol[b][t][minstoparr - 1];
									}
									else {
										nextstop = b_xsol[b][t][minstoparr + 1];
										prevstop = b_xsol[b][t][minstoparr];
									}
									cout << " is currently driving between stop " << prevstop << " and stop " << nextstop << " , current capacity is " << CBus;
									c_cost = insertstop(route, timetable, N, M, S, OG_R, OG_R1, OG_R2, OGxt, d_dl, d_de, d_ae, d_al, d_t, short_route, pickup, OG_departures, OG_arrivals, best_route,
										traveltimes, traveltimep, closestPS, dw, FC, b_xsol, b_Dsol, b_ysol, pt, b, t, current_time, true, prevstop, nextstop, max_wait, b_stop, c1, c2, c3, 0);
								}
								else {
									//cout << " is not currently driving ";
									c_cost = insertstop(route, timetable, N, M, S, OG_R, OG_R1, OG_R2, OGxt, d_dl, d_de, d_ae, d_al, d_t, short_route, pickup, OG_departures, OG_arrivals, best_route,
										traveltimes, traveltimep, closestPS, dw, FC, b_xsol, b_Dsol, b_ysol, pt, b, t, current_time, false, -1, -1, max_wait, b_stop, c1, c2, c3, 0);
								}
								//cout << endl;
							}
						}
						// Choose best feasible added cost 
						if (b_cost > c_cost && c_cost != -1) {
							b_cost = c_cost;
							b_route.clear();
							b_timetable.clear();
							nBus = route.size();
							for (j = 0; j < nBus; j++) {
								b_route.push_back(route[j]);
								b_timetable.push_back(timetable[j]);
							}

							b_bus = b;
							b_trip = t;
							b_s = b_stop;

							bestdriving = currdrive;
						}
					}
				}
			}

			// Reject if needed
			if (b_cost != INT32_MAX) {
				if(!bestdriving) cout << "\nPassenger " << pt << " accepted, assigned to bus " <<b_bus << " trip " << b_trip << " and stop " << b_s << endl << endl;
				else cout << "\nPassenger " << pt << " accepted, assigned to bus " << b_bus << " trip " << b_trip << "(DRIVING) and stop " << b_s << endl << endl;
				b_xsol[b_bus][b_trip].clear();
				b_Dsol[b_bus][b_trip].clear();
				nBus = b_route.size();
				for (j = 0; j < nBus; j++) {
					b_xsol[b_bus][b_trip].push_back(b_route[j]);
					b_Dsol[b_bus][b_trip].push_back(b_timetable[j]);
				}

				b_ysol[pt][0] = b_bus;
				b_ysol[pt][1] = b_trip;
				b_ysol[pt][2] = b_s;

				NN = b_xsol[b_bus][b_trip].size();
				for (i = 0; i < NN; i++) {
					if (b_xsol[b_bus][b_trip][i] == b_s) {
						pickup[pt] = b_Dsol[b_bus][b_trip][i];
						pickupstops[pt] = b_xsol[b_bus][b_trip][i];
						break;
					}
				}

				cost_temp = printpluscost(b_xsol, b_Dsol, b_ysol, B, OG_R, OG_R1, N, traveltimes, traveltimep, pickup, OG_departures, OG_arrivals, c1, c2, c3, OGxt, 
					d_ae, d_al, d_dl, d_de, dw, penalty, short_route, closestPS, d_t, false, isFEAS);

			}
			else {
				cout << "\nPassenger " << pt << " rejected by insertion\n";
				bestdriving = false;

				if (closestPS[pt][0] == N - 1) {
					cout << " but cloest stop is destination -> accept \n";
					bestdriving = true;
				}
			}
			
			// ++++++++++++++++++++++++++++++++++++++++++ IMPROVEMENT FUNCTION  ++++++++++++++++++++++++++++++++++++++++
			if (!bestdriving) {// only if the passenger is not assigned to a bus that is already driving or if the insertion could not find an assignment
				midpoint = clock();
				current_time += (double)(clock() - start_time) / CLK_TCK;
				
				Pa.clear();
				Pa.push_back(pt); // list of passengers with no fixed request yet
				//solution until the current time
				int pass_onboard = 0, bus_tot =0;
				for (b = 0; b < B; b++) {
					xsol[b].clear();
					Dsol[b].clear();
					X1 = b_xsol[b].size();
					for (t = 0; t < X1; t++) {
						bus_tot++;
						if (b_Dsol[b][t][0] < current_time) {
							pass_onboard++;
							
							X2 = b_xsol[b][t].size();
							vector < int > x2;
							vector < double > D2;
							for (l = 0; l < X2; l++) {
								x2.push_back(b_xsol[b][t][l]);
								D2.push_back(b_Dsol[b][t][l]);
							}
							xsol[b].push_back(x2);
							Dsol[b].push_back(D2);
						}
						else break;
					}
				}
				cout << " Bus trips fixed: " << pass_onboard << " vs total trips: " << bus_tot << endl;
				/*
				cout << "XSOL\n";
				for (b = 0; b < B; b++) {
					X1 = xsol[b].size();
					cout << "Bus " << b << endl;
					for (t = 0; t < X1; t++) {
						X2 = b_xsol[b][t].size();
						cout << " Trip " << t << endl;
						for (l = 0; l < X2; l++) {
							cout << round(Dsol[b][t][l]/60) << " ";
						}
						cout << endl;
					}
				}
				*/
				for (i = 0; i < OG_R; i++) {
					b = b_ysol[i][0];
					t = b_ysol[i][1];
					if (b >= 0) {
						if (t < xsol[b].size()) {
							ysol[i][0] = b;
							ysol[i][1] = t;
							ysol[i][2] = b_ysol[i][2];
						}
						else {
							ysol[i][0] = -3;
							ysol[i][1] = -3;
							ysol[i][2] = -3;
						}
					}
					else {
						ysol[i][0] = -1;
						ysol[i][1] = -1;
						ysol[i][2] = -1;
					}
				}
				X1 = Pa.size();
				for (i = 0; i < X1; i++) {
					ysol[Pa[i]][0] = -2;
				}
				//printpluscost(xsol, Dsol, ysol, B, OG_R, OG_R1, N, traveltimes, traveltimep, pickup, OG_departures, OG_arrivals, c1, c2, c3, OGxt, d_ae, d_al, d_dl, d_de, dw, penalty, short_route, closestPS, d_t, true);
				
				//available times and number of trip
				for (b = 0; b < B; b++) {
					trips[b] = Dsol[b].size();
					if (trips[b] != 0) bd[b] = Dsol[b][trips[b] - 1].back() + short_route;
					else bd[b] = minTS - 60 * 50;
				}
			
				for (j = 0; j < N; j++) {
					freqN[j] = -1;
					for (b = 0; b < B; b++) {
						t = Dsol[b].size() - 1;
						if (t >= 0) {
							X1 = xsol[b][t].size();
							for (l = 0; l < X1; l++) {
								if (xsol[b][t][l] == j) {
									if (freqN[j] < Dsol[b][t][l]) {
										freqN[j] = Dsol[b][t][l];
									}
									break;
								}
							}
						}
					}
				}
				max_runtime = max_request_wait - (current_time - temptimestamp[p]);
				
				cost = Improvement(B, N, M, S, OG_R, OG_R1, OG_R2, OGxt, C_OG, d_dl, d_de, d_ae, d_al, d_t, short_route, pickup, OG_departures, OG_arrivals, best_route,
					traveltimes, traveltimep, closestPS, dw, xsol, Dsol, ysol, max_wait, c1, c2, c3, endtime, trips, bd, freqN, pickupstops, penalty, N_it, timestamps,max_runtime);
				//if there is an actual improvement or the rejected passenger is accepted now then accept the improvement
				if ((cost < cost_temp || b_cost == INT32_MAX) && cost != INT32_MAX) {
					cout << " ********************* IMPROVEMENT ACCEPTED ************************\n";
					cout << " old cost=" << cost_temp << " new cost=" << cost << endl;
					for (b = 0; b < B; b++) {
						b_xsol[b].clear();
						b_Dsol[b].clear();
						X1 = xsol[b].size();
						for (t = 0; t < X1; t++) {
							X2 = xsol[b][t].size();
							vector < int > x2;
							vector < double > D2;
							for (l = 0; l < X2; l++) {
								x2.push_back(xsol[b][t][l]);
								D2.push_back(Dsol[b][t][l]);
							}
							b_xsol[b].push_back(x2);
							b_Dsol[b].push_back(D2);
						}
					}
					for (i = 0; i < OG_R; i++) {
						b_ysol[i][0] = ysol[i][0];
						b_ysol[i][1] = ysol[i][1];
						b_ysol[i][2] = ysol[i][2];
					}
					X1 = Pa.size();
					for (j = 0; j < X1; j++) {
						b_bus = b_ysol[Pa[j]][0];
						b_trip = b_ysol[Pa[j]][1];
						b_s = b_ysol[Pa[j]][2];
						NN = b_xsol[b_bus][b_trip].size();
						for (i = 0; i < NN; i++) {
							if (b_xsol[b_bus][b_trip][i] == b_s) {
								pickup[Pa[j]] = b_Dsol[b_bus][b_trip][i];
								pickupstops[Pa[j]] = b_xsol[b_bus][b_trip][i];
								break;
							}
						}
					}
				}
			}

			elapsed_time = (double)(clock() - start_time) / CLK_TCK;
			rt_p << elapsed_time << endl;
			obj_p << min(cost,cost_temp) << endl;
			current_time += (double)(clock() - midpoint) / CLK_TCK;
			//printpluscost(b_xsol, b_Dsol, b_ysol, B, OG_R, OG_R1, N, traveltimes, traveltimep, pickup, OG_departures, OG_arrivals, c1, c2, c3, OGxt, d_ae, d_al,
				//d_dl, d_de, dw, penalty, short_route, closestPS, d_t, true, isFEAS);
		}
	}
	rt_p.close();
	obj_p.close();

	cost = printpluscost(b_xsol, b_Dsol, b_ysol, B, OG_R, OG_R1, N, traveltimes, traveltimep, pickup, OG_departures, OG_arrivals, c1, c2, c3, OGxt, d_ae, d_al, 
		d_dl, d_de, dw, penalty, short_route, closestPS, d_t, true,isFEAS);
	if (isFEAS) {
		cout << "\t\t+++++++++++++ IS FEASIBLE +++++++++++++++++++\n";
		ofstream xsol_p("data/output/xsol_" + to_string(instance) + ".txt");
		ofstream ysol_p("data/output/ysol_" + to_string(instance) + ".txt");
		ofstream dsol_p("Cdata/output/dsol_" + to_string(instance) + ".txt");
		int onb = 0;
		for (i = 0; i < b_xsol.size(); i++) {
			xsol_p << "BUS " << i << endl;
			dsol_p << "BUS " << i << endl;
			for (j = 0; j < b_xsol[i].size(); j++) {
				onb = 0;
				for (p = 0; p < OG_R; p++) {
					if (b_ysol[p][0] == i && b_ysol[p][1] == j) {
						onb++;
					}
				}
				for (k = 0; k < b_xsol[i][j].size(); k++) {
					xsol_p << b_xsol[i][j][k] << "\t";
				}
				xsol_p << endl;
				for (k = 0; k < b_xsol[i][j].size(); k++) {
					dsol_p << b_Dsol[i][j][k] << "\t";
				}
				dsol_p << endl;
			}
		}
		ysol_p << "Bus" << "\t" << "Trip" << "\t" << "stop" << endl;
		for (p = 0; p < OG_R; p++) {
			b = b_ysol[p][0];
			t = b_ysol[p][1];
			s = b_ysol[p][2];
			ysol_p << b << "\t" << t << "\t" << s << endl;
		}
		xsol_p.close();
		ysol_p.close();
		dsol_p.close();
	}
	else iruns--;

	/*
	Pa.clear();
	for (p = 0; p < OG_R; p++) {
		Pa.push_back(p); // list of passengers with no fixed request yet
	}

	//solution until the timestamp
	for (b = 0; b < B; b++) {
		xsol[b].clear();
		Dsol[b].clear();
	}
		
	X1 = Pa.size();
	for (i = 0; i < X1; i++) {
		ysol[Pa[i]][0] = -2;
		pickup[i] = -1;
		pickupstops[i] = -1;
	}
	//printpluscost(xsol, Dsol, ysol, B, OG_R, OG_R1, N, traveltimes, traveltimep, pickup, OG_departures, OG_arrivals, c1, c2, c3, OGxt, d_ae, d_al, d_dl, d_de, dw, penalty, short_route, closestPS, d_t, true);

	//available times and number of trip
	for (b = 0; b < B; b++) {
		trips[b] = 0;
		bd[b] = minTS - 60 * 50;
	}

	for (j = 0; j < N; j++) {
		freqN[j] = -1;
	}

	StaticOpt(B, N, M, S, OG_R, OG_R1, OG_R2, OGxt, C_OG, d_dl, d_de, d_ae, d_al, TS, short_route, pickup, OG_departures, OG_arrivals, best_route, traveltimes, traveltimep, closestPS, dw, xsol, Dsol, ysol, max_wait, c1, c2, c3, endtime, trips, bd, freqN, pickupstops, penalty, N_it*10, 0.01, 12,0.15);
	cost = printpluscost(xsol, Dsol, ysol, B, OG_R, OG_R1, N, traveltimes, traveltimep, pickup, OG_departures, OG_arrivals, c1, c2, c3, OGxt, d_ae, d_al,d_dl, d_de, dw, penalty, short_route, closestPS, d_t, true, isFEAS);
	if (isFEAS) cout << "\t\t+++++++++++++ IS FEASIBLE +++++++++++++++++++\n";
	*/

	////////////////////////////////////////////////////// remove memory
	for (i = 0; i < S; i++) {
		delete []closestS[i];
		delete []traveltimes[i];
	}

	for (i = 0; i < OG_R; i++) {
		delete []b_ysol[i];
		delete []ysol[i];
		delete []closestPS[i];
		delete []traveltimep[i];
	}
	delete []ysol;
	delete []b_ysol;
	delete []closestPS;
	delete []closestS;
	delete []traveltimes;
	delete []traveltimep;
	}

}