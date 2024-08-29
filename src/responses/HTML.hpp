#pragma once

#include "../Response.hpp"

#include <sstream>
#include <string>
#include <vector>

typedef struct _HTMLElementAttributes {
    std::string _id;
    std::string _class;
    std::string _style;

    [[nodiscard]] bool base() const {
        if (this->_id.empty() && this->_class.empty() && this->_style.empty())
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
        elementName(std::move(name)),
        attributes(std::move(attributes)),
        content(std::move(content)),
        children(std::move(children)) {}

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
    explicit name(HTMLElementAttributes attrib = {}, std::vector<HTMLElement*> children = {}) : \
        HTMLElement(#name, std::move(attrib), "", std::move(children)) {} \
};

#define DEFINE_TEXT_ELEMENT(name) \
class name : public HTMLElement { \
public: \
    explicit name(HTMLElementAttributes attributes = {}, std::string content = "", std::vector<HTMLElement*> children = {}) : \
        HTMLElement(#name, std::move(attributes), std::move(content), std::move(children)) {} \
    explicit name(std::string content = "", std::vector<HTMLElement*> children = {}) : \
        HTMLElement(#name, {}, std::move(content), std::move(children)) {} \
};


static std::string CRLF = "\r\n";

class HTML final : public ResponseContent {
private:
    HTMLElement *root;

    std::string recurseRoot(HTMLElement *root) const {
        if (!root) return "";

        std::stringstream output;

        HTMLElementAttributes attributes = root->getAttributes();
        if (attributes.base()) {
            output << "<" << root->getElementName() << ">" << CRLF;
        } else {
            output << "<" << root->getElementName() << " ";

            if (!attributes._style.empty())
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

    void recursiveDelete(HTMLElement *root) {
        if (!root) return;

        for (auto child : root->getChildren()) {
            recursiveDelete(child);
        }

        delete root;
    }

public:
    explicit HTML(HTMLElement *root) : root(root) {}
    ~HTML() override {
        this->recursiveDelete(this->root);
    };

    [[nodiscard]] std::string raw() const override {
        std::stringstream output;

        output << "<!DOCTYPE html>" << CRLF;
        output << recurseRoot(root);

        return output.str();
    }

    [[nodiscard]] std::size_t length() const override {
        return raw().size();
    }

    [[nodiscard]] std::string mime() const noexcept override {
        return "text/html; charset=utf-8";
    }

    /*
    [[nodiscard]] const std::string mime() const override {
        std::string("text/html; charset=utf-8");
    }*/
};

typedef HTMLElementAttributes attrib;

#define $ new

DEFINE_BASIC_ELEMENT(html);
DEFINE_BASIC_ELEMENT(head);
DEFINE_BASIC_ELEMENT(body);

DEFINE_TEXT_ELEMENT(title)

DEFINE_TEXT_ELEMENT(code);

DEFINE_TEXT_ELEMENT(p);
DEFINE_TEXT_ELEMENT(h1);
DEFINE_TEXT_ELEMENT(h2);
DEFINE_TEXT_ELEMENT(h3);
DEFINE_TEXT_ELEMENT(h4);

