#pragma once

#include <string>

class ISmartSection {
	bool m_Active;
public:
	ISmartSection() : m_Active(true) {};
	virtual ~ISmartSection() {};
	virtual std::wstring filter(const std::wstring& source) = 0;
	virtual std::wstring name() const = 0;

	bool isActive() const {
		return m_Active;
	};
	void setActive(bool active) {
		m_Active = active;
	}
};

