#include "Gameplay/StickCollection/StickCollectionController.h"
#include "Gameplay/StickCollection/StickCollectionModel.h"
#include "Gameplay/StickCollection/StickCollectionView.h"
#include "Gameplay/StickCollection/Stick.h"
#include "Gameplay/GameplayService.h"
#include "Global/ServiceLocator.h"
#include <iostream>
#include <random>

namespace Gameplay {
	namespace Collection
	{
		using namespace UI::UIElement;
		using namespace Global;
		using namespace Graphics;

		StickCollectionController::StickCollectionController()
		{
			collection_view = new StickCollectionView();
			collection_model = new StickCollectionModel();
			initializeSticksArray();
		}

		StickCollectionController::~StickCollectionController()
		{
			destory();
		}

		void StickCollectionController::initializeSticksArray()
		{
			for (int i = 0; i < collection_model->number_of_elements; i++)
			{
				sticks.push_back(new Stick(i));
			}
		}

		void StickCollectionController::initialize()
		{
			initializeSticks();
			reset();
		}

		void StickCollectionController::initializeSticks()
		{
			float rectangle_width = calculateStickWidth();

			for (int i = 0; i < collection_model->number_of_elements; i++)
			{
				float rectangle_height = calculateStickHeight(i);

				sf::Vector2f rectangle_size = sf::Vector2f(rectangle_width, rectangle_height);

				sticks[i]->stick_view->initialize(rectangle_size, sf::Vector2f(0,0),0,collection_model->element_color);
			}
		}

		float StickCollectionController::calculateStickWidth()
		{
			float total_space = static_cast<float>(ServiceLocator::getInstance()->getGraphicService()->getGameWindow()->getSize().x);

			float total_spacing = collection_model->space_percentage * total_space;

			float space_between = total_spacing / (collection_model->number_of_elements - 1);
			collection_model->setElementSpacing(space_between);

			float remaining__space = total_space - total_spacing;

			float rectangle_width = remaining__space / collection_model->number_of_elements;

			return rectangle_width;
		}

		float StickCollectionController::calculateStickHeight(int array_pos)
		{
			return (static_cast<float>(array_pos+1) / collection_model->number_of_elements) * collection_model->max_element_height;
		}

		void StickCollectionController::shuffleSticks()
		{
			std::random_device device;
			std::mt19937 random_engine(device());

			std::shuffle(sticks.begin(), sticks.end(), random_engine);
		}

		void StickCollectionController::resetSearchStick()
		{
			stick_to_search = sticks[std::rand() % sticks.size()];
			stick_to_search->stick_view->setFillColor(collection_model->search_element_color);
		}

		void StickCollectionController::processSearchThreadState()
		{
			if (search_thread.joinable() && stick_to_search == nullptr)
			{
				joinThreads();
			}
		}

		void StickCollectionController::processLinearSearch()
		{
			Sounds::SoundService* sound_service = Global::ServiceLocator::getInstance()->getSoundService();

			for (int i = 0; i < sticks.size(); i++)
			{

				number_of_array_access += 1;
				number_of_comparisons++;

				sound_service->playSound(Sounds::SoundType::COMPARE_SFX);

				if (sticks[i] == stick_to_search)
				{
					stick_to_search->stick_view->setFillColor(collection_model->found_element_color);
					stick_to_search = nullptr;
					return;
				}
				else
				{
					sticks[i]->stick_view->setFillColor(collection_model->processing_element_color);
					std::this_thread::sleep_for(std::chrono::milliseconds(current_operation_delay));
					sticks[i]->stick_view->setFillColor(collection_model->element_color);
				}

			}
		}

		void StickCollectionController::resetVariables()
		{
			number_of_comparisons = 0;
			number_of_array_access = 0;
		}

		void StickCollectionController::joinThreads()
		{
			search_thread.join();
		}

		void StickCollectionController::reset()
		{
			current_operation_delay = 0;

			shuffleSticks();
			updateSticksPosition();
			resetSticksColor();
			resetSearchStick();
			resetVariables();
		}

		void StickCollectionController::updateSticksPosition()
		{
			for (int i = 0; i < sticks.size(); i++)
			{
				float x_position = (i * sticks[i]->stick_view->getSize().x) + ((i + 1) * collection_model->elements_spacing);
				float y_position = collection_model->element_y_position - sticks[i]->stick_view->getSize().y;

				sticks[i]->stick_view->setPosition(sf::Vector2f(x_position, y_position));
			}
		}

		void StickCollectionController::resetSticksColor()
		{
			for (int i = 0; i < sticks.size(); i++)
			{
				sticks[i]->stick_view->setFillColor(collection_model->element_color);
			}
		}

		void StickCollectionController::update()
		{
			for (int i = 0; i < sticks.size(); i++)
				sticks[i]->stick_view->update();
		}

		void StickCollectionController::render()
		{
			for (int i = 0; i < sticks.size(); i++)
				sticks[i]->stick_view->render();
		}

		void StickCollectionController::searchElement(SearchType search_type)
		{
			this->search_type = search_type;
 
			switch (search_type)
			{
			case Gameplay::Collection::SearchType::LINEAR_SEARCH:
				current_operation_delay = collection_model->linear_search_delay;
				time_complexity = "O(n)";
				search_thread = std::thread(&StickCollectionController::processLinearSearch, this);
				//processLinearSearch();
				break;
			}
		}

		SearchType StickCollectionController::getSearchType()
		{
			return search_type;
		}

		int StickCollectionController::getNumberOfSticks()
		{
			return collection_model->number_of_elements;
		}

		int StickCollectionController::getDelayMilliseconds()
		{
			return current_operation_delay;
		}

		sf::String StickCollectionController::getTimeComplexity()
		{
			return time_complexity;
		}

		int StickCollectionController::getNumberOfComparisons()
		{
			return number_of_comparisons;
		}

		int StickCollectionController::getNumberOfArrayAccess()
		{
			return number_of_array_access;
		}

		void StickCollectionController::destory()
		{
			if (search_thread.joinable())
			{
				search_thread.join();
			}

			for (int i = 0; i < sticks.size(); i++) 
				delete(sticks[i]);
			
			sticks.clear();

			delete (collection_view);
			delete (collection_model);
		}
	}
}