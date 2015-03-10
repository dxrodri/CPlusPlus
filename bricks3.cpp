#ifndef _BRICKS_CPP_
#define _BRICKS_CPP_

#define BRICK_SIZE_2  2
#define BRICK_SIZE_3  3
#define DEBUG
//#define TRACE

#include <stdlib.h>
#include <iostream>
#include <vector>
#include <deque>
#include <map>
#include <set>
#include <numeric>
#include <time.h>

using std::cout;
using std::endl;
using std::vector;
using std::deque;
using std::map;
using std::pair;
using std::set;
using std::accumulate;
using std::fixed;

static vector<deque<int> > SIZE_ZERO_BRICK_LAYER = { { } };
static vector<deque<int> > SIZE_TWO_BRICK_LAYER = { { BRICK_SIZE_2 } };
static vector<deque<int>> SIZE_THREE_BRICK_LAYER = { { BRICK_SIZE_3 } };

class BrickLayerData {
private:
	int noOfValidBrickPatterns;
	vector<deque<int>> validBaseLayers;
	vector<set<int>> brickJointPositions;
	vector<vector<int>> brickNextLayerIds;
public:
	void setValidBaseLayers(vector<deque<int>> validBaseLayers) {
		this->validBaseLayers = validBaseLayers;
	}
	vector<deque<int>> getValidBaseLayers() {
		return validBaseLayers;
	}
	void setBrickJointPositions(vector<set<int>> brickJointPositions) {
		this->brickJointPositions = brickJointPositions;
	}
	vector<set<int>> getBrickJointPositions() {
		return brickJointPositions;
	}
	void setNumberOfValidBasePatterns(int noOfValidBrickPatterns) {
		this->noOfValidBrickPatterns = noOfValidBrickPatterns;
	}
	int getNumberOfValidBasePatterns() {
		return noOfValidBrickPatterns;
	}
	void setBrickNextLayerIds(vector<vector<int>> brickNextLayerIds) {
		this->brickNextLayerIds = brickNextLayerIds;
	}
	vector<vector<int>> getBrickNextLayerIds() {
		return brickNextLayerIds;
	}
};

/*Holds brick layer data for given wall Length*/
static map<int, BrickLayerData> brickLayerDataMap;

class BrickLayerDataHelper {
private:
	static vector<deque<int>> getValidBaseLayers(int wallLength);
	static vector<set<int>> getBrickJointPositions(
			vector<deque<int>> validBaseLayers);
	static BrickLayerData getBrickLayerData(int wallLenth);
	static vector<vector<int>> getNextLayerIds(
			vector<set<int>> brickJointPositions);
	int getBrickLayerCombinationCount(int brickLayer, int wallHeight);
public:
	static double getBrickLayerCombinations(int wallLength, int wallHeight);
};

/*Get Brick Combinations - main function*/
double BrickLayerDataHelper::getBrickLayerCombinations(int wallLength,
		int wallHeight) {
	BrickLayerData brickLayerData = getBrickLayerData(wallLength);
	int numberOfBaseLayerPatterns =
			brickLayerData.getNumberOfValidBasePatterns();
	vector<double> * possibleWallCombinations = new vector<double>(
			numberOfBaseLayerPatterns, 1); //Initialize with 1s for the first layer
	vector<double> * possibleWallCombinationsTemp = new vector<double>();
	vector<double> * possibleWallCombinationsPlaceHolder;
	vector<vector<int>> brickNextLayerIds =
			brickLayerData.getBrickNextLayerIds();
	//For each height level, use previous level data
	for (int i = 1; i < wallHeight; i++) {
		for (int j = 0; j < numberOfBaseLayerPatterns; j++) {
			vector<int> nextLayerIds = brickNextLayerIds[j];
			double count = 0;
			for (vector<int>::const_iterator it = nextLayerIds.begin();
					it != nextLayerIds.end(); ++it) {
				count += (*possibleWallCombinations)[*it];
			}
			(*possibleWallCombinationsTemp).push_back(count);
		}
#ifdef TRACE
		cout << "At height " << i << " brick combinations " << accumulate((*possibleWallCombinationsTemp).begin(), (*possibleWallCombinationsTemp).end(), 0) << endl;
#endif
		possibleWallCombinationsPlaceHolder = possibleWallCombinations;
		possibleWallCombinations = possibleWallCombinationsTemp;
		possibleWallCombinationsTemp = possibleWallCombinationsPlaceHolder;
		(*possibleWallCombinationsTemp).clear();
	}
	return accumulate((*possibleWallCombinations).begin(),
			(*possibleWallCombinations).end(), 0.0);
}

/*Get Brick layer data for a given wallLength*/
BrickLayerData BrickLayerDataHelper::getBrickLayerData(int wallLength) {
#ifdef DEBUG
	cout << " In getBrickLayerData " << endl;
	clock_t startTime = clock();
#endif
	map<int, BrickLayerData>::iterator it = brickLayerDataMap.find(wallLength);
	if (it != brickLayerDataMap.end()) {
		return it->second;
	} else {
		BrickLayerData brickLayerData;
#ifdef DEBUG
		cout << " Calling getValidBaseLayers " << endl;
		clock_t startTime1 = clock();
#endif
		vector<deque<int>> validBaseLayers = getValidBaseLayers(wallLength);
#ifdef DEBUG
		cout << " Returned from getValidBaseLayers in  "
				<< double(clock() - startTime1) / (double) CLOCKS_PER_SEC
				<< " seconds." << endl;
#endif
		int noOfValidBrickPatterns = validBaseLayers.size();
#ifdef DEBUG
		cout << " Number of base valid patterns is " << noOfValidBrickPatterns
				<< endl;
#endif
#ifdef TRACE
		for (vector<deque<int>>::const_iterator it = validBaseLayers.begin(); it != validBaseLayers.end(); ++it) {
			deque<int> validLayer = (*it);
			for (deque<int>::const_iterator it1 = validLayer.begin(); it1 != validLayer.end(); ++it1) {
				cout << *it1 << ",";
			}
			cout << endl;
		}
		cout << endl;
#endif
		brickLayerData.setNumberOfValidBasePatterns(noOfValidBrickPatterns);
		brickLayerData.setValidBaseLayers(validBaseLayers);
		vector<set<int>> brickJointPositions = getBrickJointPositions(
				validBaseLayers);
		brickLayerData.setBrickJointPositions(brickJointPositions);
		vector<vector<int>> brickNextLayerIds = getNextLayerIds(
				brickJointPositions);
		brickLayerData.setBrickNextLayerIds(brickNextLayerIds);
		brickLayerDataMap.insert(
				pair<int, BrickLayerData>(wallLength, brickLayerData));
#ifdef DEBUG
		cout << " Out from getBrickLayerData in  "
				<< double(clock() - startTime) / (double) CLOCKS_PER_SEC
				<< " seconds." << endl;
#endif
		return brickLayerData;
	}
}

/*Get valid base/core brick layers*/
vector<deque<int>> BrickLayerDataHelper::getValidBaseLayers(int wallLength) {
	if (wallLength == 0 || wallLength == 1) {
		return SIZE_ZERO_BRICK_LAYER;
	} else {
		if (wallLength == 2) {
			return SIZE_TWO_BRICK_LAYER;
		} else {
			if (wallLength == 3) {
				return SIZE_THREE_BRICK_LAYER;
			} else {
				vector<deque<int>> baseLayers;
				vector<deque<int>> startWithSizeTwoBrickLayer =
						BrickLayerDataHelper::getValidBaseLayers(
								wallLength - BRICK_SIZE_2);
				for (vector<deque<int>>::iterator it =
						startWithSizeTwoBrickLayer.begin();
						it != startWithSizeTwoBrickLayer.end(); ++it) {
					deque<int> baseLayer = *it;
					if (it->size() != 0) {
						baseLayer.push_front(BRICK_SIZE_2);
						baseLayers.push_back(baseLayer);
					}
				}
				vector<deque<int>> startWithSizeThreeBrickLayer =
						BrickLayerDataHelper::getValidBaseLayers(
								wallLength - BRICK_SIZE_3);
				for (vector<deque<int>>::iterator it =
						startWithSizeThreeBrickLayer.begin();
						it != startWithSizeThreeBrickLayer.end(); ++it) {
					deque<int> baseLayer = *it;
					if (it->size() != 0) {
						baseLayer.push_front(BRICK_SIZE_3);
						baseLayers.push_back(baseLayer);
					}
				}
				return baseLayers;
			}
		}
	}
}

/* Get brick joint positions*/
vector<set<int>> BrickLayerDataHelper::getBrickJointPositions(
		vector<deque<int>> validBrickPatterns) {
#ifdef DEBUG
	cout << " In getBrickJointPositions " << endl;
	clock_t startTime = clock();
#endif
	vector<set<int>> brickJointPositions;
	for (vector<deque<int>>::const_iterator it = validBrickPatterns.begin();
			it != validBrickPatterns.end(); ++it) {
		deque<int> validLayer = *it;
		set<int> jointPosition;
		int currentJointPosition = 0;
		for (deque<int>::const_iterator it1 = validLayer.begin();
				it1 != validLayer.end() - 1; ++it1) {
			currentJointPosition += *it1;
			jointPosition.insert(currentJointPosition);
		}
		brickJointPositions.push_back(jointPosition);
	}
#ifdef TRACE
	cout << "Brick joint positions " << endl;
	for (vector<set<int>>::const_iterator it = brickJointPositions.begin(); it != brickJointPositions.end(); ++it) {
		set<int> jointPosition = (*it);
		for (set<int>::const_iterator it1 = jointPosition.begin(); it1 != jointPosition.end(); ++it1) {
			cout << *it1 << ",";
		}
		cout << endl;
	}
	cout << endl;
#endif
#ifdef DEBUG
	cout << " Out from getBrickJointPositions in  "
			<< double(clock() - startTime) / (double) CLOCKS_PER_SEC
			<< " seconds." << endl;
#endif
	return brickJointPositions;
}
static long double timeSpentInEmptyIntersection = 0.0;
/* empty intersection check of two sets - true if no elements intersect*/
static bool empty_intersection(const set<int>& x, const set<int>& y) {
	set<int>::const_iterator i = x.begin();
	set<int>::const_iterator j = y.begin();
	while (i != x.end() && j != y.end()) {
		if (*i < *j)
			++i;
		else if (*j < *i)
			++j;
		else {
			return false;
		}
	}
	return true;
}

/* Get next brick layers layer than can be placed without cracks*/
vector<vector<int>> BrickLayerDataHelper::getNextLayerIds(
		vector<set<int>> brickJointPositions) {
#ifdef DEBUG
	cout << " In getNextLayerIds " << endl;
	clock_t startTime = clock();
#endif
	//find next layer without crack joint
	vector<vector<int>> nextLayerIds(brickJointPositions.size());
	;
	int i = 0;
	for (vector<set<int>>::const_iterator it = brickJointPositions.begin();
			it != brickJointPositions.end(); ++it, i++) {
		set<int> brickJointPosition = *it;
		vector<int> validNextLayers = nextLayerIds[i];
		int j = i + 1;
		for (vector<set<int>>::const_iterator it1 = brickJointPositions.begin()
				+ j; it1 != brickJointPositions.end(); ++it1, j++) {
#ifdef TRACE
			cout << " Locating next layers for base " << i << " at position " << j << endl;
#endif
			set<int> brickJointPositionNext = *it1;
			if (empty_intersection(brickJointPosition,
					brickJointPositionNext)) {
				//if (is_disjoint(brickJointPosition, brickJointPositionNext)){
				validNextLayers.push_back(j);
				vector<int> validNextLayersLinked = nextLayerIds[j];
				validNextLayersLinked.push_back(i);
				nextLayerIds[j] = validNextLayersLinked;
			}
		}
		nextLayerIds[i] = validNextLayers;
	}
#ifdef TRACE
	cout << "Valid Next Layer Ids " << endl;
	for (vector<vector<int>>::const_iterator it = nextLayerIds.begin(); it != nextLayerIds.end(); ++it) {
		vector<int> validNextLaterIds = (*it);
		for (vector<int>::const_iterator it1 = validNextLaterIds.begin(); it1 != validNextLaterIds.end(); ++it1) {
			cout << *it1 << ",";
		}
		cout << endl;
	}
	cout << endl;
#endif
#ifdef DEBUG
	cout << " Out from getNextLayerIds in  "
			<< double(clock() - startTime) / (double) CLOCKS_PER_SEC
			<< " seconds." << endl;
	cout << " Time spent in intersection check " << timeSpentInEmptyIntersection
			<< endl;
#endif
	return nextLayerIds;
}

int main(int argc, char* argv[]) {
	int wallLength;
	int wallHeight;
	if (argc == 3) {
		wallLength = atoi(argv[1]);
		wallHeight = atoi(argv[2]);
	} else {
		cout << "Input parameters need to be wallLength and wallHeight. Using 32 and 10" << endl;
		//For testing
		wallLength = 32;
		wallHeight = 10;
	}
#ifdef DEBUG
	cout << " Calling getBrickLayerCombinations " << endl;
	clock_t startTime = clock();
#endif
	double brickLayerCombinations =
			BrickLayerDataHelper::getBrickLayerCombinations(wallLength,
					wallHeight);
#ifdef DEBUG
	cout << " Returned from getBrickLayerCombinations in  "
			<< double(clock() - startTime) / (double) CLOCKS_PER_SEC
			<< " seconds." << endl;
#endif
	cout.precision(0);
	cout << "C(" << wallLength << "," << wallHeight << ")="
			<<  fixed << brickLayerCombinations << endl;

	return 0;
}
#endif
