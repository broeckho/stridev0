#ifndef SUBJECT_H_INCLUDED
#define SUBJECT_H_INCLUDED
///*
// * Copyright 2011-2016 Universiteit Antwerpen
// *
// * Licensed under the EUPL, Version 1.1 or  as soon they will be approved by
// * the European Commission - subsequent versions of the EUPL (the "Licence");
// * You may not use this work except in compliance with the Licence.
// * You may obtain a copy of the Licence at: http://ec.europa.eu/idabc/eupl5
// *
// * Unless required by applicable law or agreed to in writing, software
// * distributed under the Licence is distributed on an "AS IS" basis,
// * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
// * See the Licence for the specific language governing
// * permissions and limitations under the Licence.
// */
/**
 * @file
 * Interface/Implementation of Subject.
 */

//#include <functional>
//#include <map>
//#include <memory>

namespace stride {
namespace util {

///**
// * Template for Subject/Observer (or Publish/Subscribe). Offers flexibility
// * in defining event types and having customized callbacks. Despite the
// * shared_ptrs in the Register/Unregister, the Subject takes no ownership
// * of the observer object and only stores a weak_ptr.
// */
template <typename E, typename U>
class Subject
{
public:
	using EventType = E;
	using ObserverType = U;
public:
	virtual ~Subject() { UnregisterAll(); }

	void Register(const std::shared_ptr<ObserverType>&);

	void Unregister(const std::shared_ptr<ObserverType>&);

	void UnregisterAll();

	void Notify(const EventType&);
private:
	std::vector<std::shared_ptr<ObserverType> > m_observers;

};

template <typename E, typename U>
void Subject<E, U>::Register(const std::shared_ptr<U>& u)
{
	m_observers.push_back(u);
}

template <typename E, typename U>
void Subject<E, U>::Unregister(const std::shared_ptr<U>& u)
{
	m_observers.erase(u);
}

template <typename E, typename U>
void Subject<E, U>::UnregisterAll()
{
	m_observers.clear();
}

template <typename E, typename U>
void Subject<E, U>::Notify(const EventType& e)
{
	for (std::shared_ptr<U>& o: m_observers) {
		//TODO lock
		o->Update(e);
	}
//	for (const auto& o : m_observers) {
//		const auto spt = o.first.lock();
//		if (spt) {
//			(o.second)(e);
//		} else {
//			m_observers.erase(o.first);
//		}
//	}
}

} // namespace
} // namespace

#endif
