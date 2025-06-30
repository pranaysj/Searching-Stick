#pragma once
#include <SFML/Graphics.hpp>
#include <vector>
#include <thread>
#include <SFML/Graphics/Color.hpp>


using namespace std;

namespace Gameplay {
	namespace Collection
	{
		class StickCollectionModel;
		class StickCollectionView;
		enum class SearchType;
		struct Stick;

		class StickCollectionController {

		private:
			StickCollectionView* collection_view;
			StickCollectionModel* collection_model;

			std::vector<Stick*> sticks;
			SearchType search_type;

			Stick* stick_to_search;

			int number_of_comparisons;
			int number_of_array_access;

			int current_operation_delay;

			std::thread search_thread;

			sf::String time_complexity;

			void initializeSticks();
			float calculateStickWidth();

			void updateSticksPosition();

			void resetSticksColor();
			void initializeSticksArray();
			float calculateStickHeight(int array_pos);
			
			void shuffleSticks();
			void resetSearchStick();

			void processSearchThreadState();
			void joinThreads();
			void processLinearSearch();

			void resetVariables();

			void sortElement();
			bool compareElementsByData(const Stick* a, const Stick* b) const;
			void processBinarySearch();

			void destory();

		public:
			StickCollectionController();
			~StickCollectionController();

			void initialize();
			void update();
			void render();

			void reset();

			void searchElement(SearchType search_type);

			SearchType getSearchType();

			int getNumberOfComparisons();
			int getNumberOfArrayAccess();

			int getNumberOfSticks();

			int getDelayMilliseconds();

			sf::String getTimeComplexity();

		};
	}
}