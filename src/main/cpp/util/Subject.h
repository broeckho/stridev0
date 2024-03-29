#ifndef SUBJECT_H_INCLUDED
#define SUBJECT_H_INCLUDED
/*
 * Copyright 2011-2016 Universiteit Antwerpen
 *
 * Licensed under the EUPL, Version 1.1 or  as soon they will be approved by
 * the European Commission - subsequent versions of the EUPL (the "Licence");
 * You may not use this work except in compliance with the Licence.
 * You may obtain a copy of the Licence at: http://ec.europa.eu/idabc/eupl5
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the Licence is distributed on an "AS IS" basis,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the Licence for the specific language governing
 * permissions and limitations under the Licence.
 */
/**
 * @file
 * Interface/Implementation of Subject.
 */

#include <functional>
#include <map>
#include <memory>

namespace Stride {
namespace Util {

/**
 * Template for Subject/Observer (or Publish/Subscribe). Offers flexibility
 * in defining event types and having customized callbacks. Despite the
 * shared_ptrs in the Register/Unregister, the Subject takes no ownership
 * of the observer object and only stores a weak_ptr.
 */
template <typename E>
class Subject
{
public:
	using EventType = E;
	using CallbackType = std::function<void(const EventType&)>;

public:
	virtual ~Subject() { UnregisterAll(); }

	template <typename U>
	void Register(const std::shared_ptr<U>&, CallbackType);

	template <typename U>
	void Unregister(const std::shared_ptr<U>&);

	void UnregisterAll();

	void Notify(const EventType&);

private:
	std::map<std::weak_ptr<const void>, CallbackType, std::owner_less<std::weak_ptr<const void>>> m_observers;
};

template <typename E>
template <typename U>
void Subject<E>::Register(const std::shared_ptr<U>& u, CallbackType f)
{
	m_observers.insert(make_pair(std::static_pointer_cast<const void>(u), f));
}

template <typename E>
template <typename U>
void Subject<E>::Unregister(const std::shared_ptr<U>& u)
{
	m_observers.erase(std::static_pointer_cast<const void>(u));
}

template <typename E>
void Subject<E>::UnregisterAll()
{
	m_observers.clear();
}

template <typename E>
void Subject<E>::Notify(const EventType& e)
{
	for (const auto& o : m_observers) {
		const auto spt = o.first.lock();
		if (spt) {
			(o.second)(e);
		} else {
			m_observers.erase(o.first);
		}
	}
}

} // namespace
} // namespace

#endif
