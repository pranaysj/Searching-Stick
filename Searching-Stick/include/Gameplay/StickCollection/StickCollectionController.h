#pragma once
#include <SFML/Graphics.hpp>
#include <vector>
#include <thread>


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

			void initializeSticks();
			float calculateStickWidth();

			void updateSticksPosition();

			void resetSticksColor();
			void initializeSticksArray();
			float calculateStickHeight(int array_pos);
			
			void shuffleSticks();
			void resetSearchStick();

			void processLinearSearch();

			void resetVariables();

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

			int getNumberOfSticks();
			int getNumberOfComparisons();
			int getNumberOfArrayAccess();
		};
	}
}