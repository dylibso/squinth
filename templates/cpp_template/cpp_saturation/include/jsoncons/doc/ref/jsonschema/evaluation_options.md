### jsoncons::jsonschema::evaluation_options

```cpp
#include <jsoncons_ext/jsonschema/jsonschema.hpp>

class evaluation_options;
```

<br>

Allows configuration of JSON Schema evaluation.

#### Constructors

    evaluation_options();
Constructs an `evaluation_options` with default values. 

    evaluation_options(const evaluation_options& other);
Copy constructor

#### Member functions

    evaluation_options& operator=(const evaluation_options& other);
Assignment operator

    const std::string& default_version() const;
    evaluation_options& default_version(const std::string& version); 
Get or set a default [schema version](schema_version.md). The default
version determines the schema dialect that is used if the `$schema` 
keyword is not present in the top level of the JSON Schema document. 
Defaults to `schema_version::draft202012()`.

    bool compatibility_mode() const;
    evaluation_options& compatibility_mode(bool value); 
Get or set the compatibility mode. If set to `true`, the JSON Schema 2019-09
and 2020-12 implementations support the Draft 7 "definitions" and 
"dependencies" keywords. The default is `false`. 

    bool require_format_validation() const;
    evaluation_options& require_format_validation(bool value); 
Determines whether `format` is an assertion. The default is `false`. 

#### Non-member functions

    bool operator==(const evaluation_options& lhs, const evaluation_options& rhs);
Checks if the contents of `lhs` and `rhs` are equal.

