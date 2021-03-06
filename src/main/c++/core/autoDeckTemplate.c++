#include "autoDeckTemplate.h++"
#include <errorHandling/assert.h++>
#include <errorHandling/exceptions.h++>
#include <boost/functional/hash.hpp>

namespace Tyrant {
    namespace Core {

        AutoDeckTemplate::AutoDeckTemplate(unsigned int commanderId, std::vector<unsigned int> const & cardIds)
        : StaticDeckTemplate()
        {
            this->commanderId = commanderId;
            for(unsigned int cardId : cardIds) {
                this->cards.insert(cardId);
            }
        }


        AutoDeckTemplate::AutoDeckTemplate(std::list<unsigned int> const & ids)
        : StaticDeckTemplate()
        {
            std::list<unsigned int>::const_iterator iter = ids.begin();
            unsigned int const commanderId = *iter;
            this->commanderId = commanderId;
            for(iter++ ;iter != ids.end(); iter++) {
                unsigned int cardId = *iter;
                this->cards.insert(cardId);
            }
        }


        AutoDeckTemplate::operator std::string() const
        {
            std::stringstream ssString;
            ssString << "IDS:";
            ssString << this->commanderId;
            for(std::multiset<unsigned int>::const_iterator iter = this->cards.begin()
               ;iter != this->cards.end()
               ;iter++
               )
            {
                ssString << ",";
                ssString << *iter;
            }
            return ssString.str();
        }

        CREATE_VISITOR_METHOD(AutoDeckTemplate)

        StaticDeckTemplate::Ptr
        AutoDeckTemplate::withCommander(unsigned int commanderId) const
        {
            StaticDeckTemplate::Ptr copy(new AutoDeckTemplate(*this));
            copy->commanderId = commanderId;
            return copy;
        }

        size_t
        AutoDeckTemplate::getNumberOfNonCommanderCards() const
        {
            return this->cards.size();
        }

        unsigned int
        AutoDeckTemplate::getCardIdAtIndex(size_t index) const
        {
            std::multiset<unsigned int>::const_iterator iter = this->cards.begin();
            for(size_t i = 0u; i < index; i++) {
                iter++;
            }
            return *iter;
        }

        StaticDeckTemplate::Ptr
        AutoDeckTemplate::withCard(unsigned int cardId) const
        {
            AutoDeckTemplate::Ptr copy(new AutoDeckTemplate(*this));
            copy->cards.insert(cardId);
            return copy;
        }

        StaticDeckTemplate::Ptr
        AutoDeckTemplate::withCardAtIndex(unsigned int cardId, size_t index) const
        {
            AutoDeckTemplate::Ptr copy(new AutoDeckTemplate(*this));
            copy->cards.insert(cardId);
            return copy;
        }

        StaticDeckTemplate::Ptr
        AutoDeckTemplate::withoutCardAtIndex(size_t index) const
        {
            AutoDeckTemplate::Ptr copy(new AutoDeckTemplate(*this));
            std::multiset<unsigned int>::iterator iter = copy->cards.begin();
            for(size_t i = 0u; i < index; i++) {
                iter++;
            }
            copy->cards.erase(iter);
            return copy;
        }

        StaticDeckTemplate::Ptr
        AutoDeckTemplate::withSwappedCards(size_t i, size_t j) const
        {
            return AutoDeckTemplate::Ptr(new AutoDeckTemplate(*this));
        }

        Tyrant::Core::StaticDeckTemplate::Ptr
        AutoDeckTemplate::withReplacedCardAtIndex(unsigned int cardId, size_t index) const
        {
            AutoDeckTemplate::Ptr copy(new AutoDeckTemplate(*this));
            std::multiset<unsigned int>::iterator iter = copy->cards.begin();
            for(size_t i = 0u; i < index; i++) {
                iter++;
            }
            copy->cards.erase(iter);
            copy->cards.insert(cardId);
            return copy;
        }

        bool
        operator< (AutoDeckTemplate const & a
                  ,AutoDeckTemplate const & b
                  )
        {
            if (a.commanderId < b.commanderId) {
                return true;
            } else if (a.commanderId > b.commanderId) {
                return false;
            } else {
                return a.cards < b.cards;
            }
        }

        bool
        AutoDeckTemplate::equals2(StaticDeckTemplate const & rhs) const
        {
            if (AutoDeckTemplate const * rhs2 =
                dynamic_cast<AutoDeckTemplate const *>
                    (&rhs)
               )
            {
                return this->equals2(*rhs2);
            } else {
                return false;
            }
        }

        bool
        AutoDeckTemplate::equals2(AutoDeckTemplate const & rhs) const
        {
            return StaticDeckTemplate::equals2(rhs)
                && this->commanderId == rhs.commanderId;
        }

        size_t
        AutoDeckTemplate::hashCode() const
        {
            size_t result = 0;
            boost::hash_combine(result, this->commanderId);
            boost::hash_range(result, this->cards.begin(), this->cards.end());
            return result;
        }

    }
}
