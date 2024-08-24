#pragma once

#include "Response.hpp"

#include <sstream>
#include <string>
#include <vector>

typedef struct _HTMLElementAttributes {
    std::string _id = "";
    std::string _class = "";
    std::string _style = "";

    bool base() {
        if (this->_id == "" && this->_class == "" && this->_style == "")
            return true;
        return false;
    }
} HTMLElementAttributes;

class HTMLElement {
private:
    std::string elementName;
    HTMLElementAttributes attributes;
    std::string content;
    std::vector<HTMLElement*> children;

public:
    HTMLElement(
            std::string name,
            HTMLElementAttributes attributes, 
            std::string content, 
            std::vector<HTMLElement*> children) : 
        elementName(name),
        attributes(attributes),
        content(content),
        children(children) {}

    virtual ~HTMLElement() = default;

    std::string getElementName() {
        return this->elementName;
    }

    const HTMLElementAttributes &getAttributes() {
        return this->attributes;
    }

    std::string getElementContent() {
        return this->content;
    }

    std::vector<HTMLElement*> getChildren() {
        return this->children;
    }
};

#define DEFINE_BASIC_ELEMENT(name) \
class name : public HTMLElement { \
public: \
    explicit name(std::vector<HTMLElement*> children = {}) : \
        HTMLElement(#name, {}, "", std::move(children)) {} \
    name(HTMLElementAttributes attrib = {}, std::vector<HTMLElement*> children = {}) : \
        HTMLElement(#name, std::move(attrib), "", std::move(children)) {} \
};

#define DEFINE_TEXT_ELEMENT(name) \
class name : public HTMLElement { \
public: \
    name(HTMLElementAttributes attributes = {}, std::string content = "", std::vector<HTMLElement*> children = {}) : \
        HTMLElement(#name, attributes, content, std::move(children)) {} \
    name(std::string content = "", std::vector<HTMLElement*> children = {}) : \
        HTMLElement(#name, {}, content, std::move(children)) {} \
};


static std::string CRLF = "\r\n";

class HTML : public ResponseContent {
private:
    HTMLElement *root;

    std::string recurseRoot(HTMLElement* root) {
        if (!root) return "";

        std::stringstream output;

        HTMLElementAttributes attributes = root->getAttributes();
        if (attributes.base()) {
            output << "<" << root->getElementName() << ">" << CRLF;
        } else {
            output << "<" << root->getElementName() << " ";

            if (attributes._style != "")
                output << "style=\"" << attributes._style << "\"";
            output << ">" << CRLF;
        }

        if (!root->getChildren().empty()) {
            for (const auto& child : root->getChildren()) {
                output << recurseRoot(child);
            }
        } else {
            if (!root->getElementContent().empty()) {
                output << root->getElementContent() << CRLF;
            }
        }

        output << "</" << root->getElementName() << ">" << CRLF;
        return output.str();
    }

public:
    HTML(HTMLElement *root) : root(std::move(root)) {}
    ~HTML() = default;

    std::string raw() override {
        std::stringstream output;

        output << "<!DOCTYPE html>" << CRLF;
        output << recurseRoot(root);

        return output.str();
    }

    size_t length() override {
        return raw().size();
    }
};

typedef HTMLElementAttributes attrib;

#define $ new

DEFINE_BASIC_ELEMENT(html);
DEFINE_BASIC_ELEMENT(head);
DEFINE_BASIC_ELEMENT(body);

DEFINE_TEXT_ELEMENT(title)

DEFINE_TEXT_ELEMENT(p);
DEFINE_TEXT_ELEMENT(h1);
DEFINE_TEXT_ELEMENT(h2);
DEFINE_TEXT_ELEMENT(h3);
DEFINE_TEXT_ELEMENT(h4);

