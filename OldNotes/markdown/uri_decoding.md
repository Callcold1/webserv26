# Handling Unicode in HTTP Request Lines

## 1. Structure of an HTTP Request Line
A typical HTTP request line follows this format:
```
GET /path/to/resource?query=value HTTP/1.1
```
It consists of three parts:
1. **Method** (e.g., `GET`, `POST`) → **Always ASCII**
2. **Request Target (URI/Path/Query)** → **May contain encoded Unicode**
3. **HTTP Version** (e.g., `HTTP/1.1`) → **Always ASCII**

### Where Unicode May Appear
- **The path** (e.g., `/世界.html`)
- **The query string** (e.g., `?q=über`)
- **The domain name (Host header, not in the request line itself)**

## 2. Should Unicode Be Translated to ASCII?
**Do not blindly translate Unicode to ASCII.** Instead, follow these rules:

### A. Decode Percent-Encoding (UTF-8)
URIs must be percent-encoded, so Unicode characters should be in **UTF-8 percent-encoded form**:
```
GET /%E4%B8%96%E7%95%8C.html HTTP/1.1
```
This represents `/世界.html`, where:
- `%E4%B8%96` → `世`
- `%E7%95%8C` → `界`

**Steps to Handle:**
1. **Decode percent-encoding** into UTF-8.
2. **Normalize Unicode (NFC recommended).**
3. **Process the request normally.**

### B. ASCII-Only Components
- The **method** (`GET`, `POST`, etc.) and **HTTP version** (`HTTP/1.1`) **must always remain ASCII**.
- If non-ASCII characters appear in these parts, **reject the request with `400 Bad Request`**.

### C. Domain Names (Punycode for IDNs)
- The **Host header** may contain **Unicode domains**.
- Browsers send **Internationalized Domain Names (IDNs) in Punycode**:
  ```
  Host: xn--mnchen-3ya.de  (Equivalent to münchen.de)
  ```
- Convert **Punycode → Unicode** if needed.

## 3. Recommended Approach in a Web Server
✔ **Step 1: Extract Request Line**  
✔ **Step 2: Validate ASCII-Only Parts (Method & HTTP Version)**  
✔ **Step 3: Decode Percent-Encoding (UTF-8) in URI**  
✔ **Step 4: Normalize Unicode** (use NFC form)  
✔ **Step 5: Process the Request**  

## 4. Example: Parsing a Unicode Request in C++98
### UTF-8 Percent-Decoding Function
```cpp
#include <iostream>
#include <string>

// Decode percent-encoded UTF-8
std::string percent_decode(const std::string& encoded) {
    std::string decoded;
    size_t i = 0;
    while (i < encoded.length()) {
        if (encoded[i] == '%' && i + 2 < encoded.length()) {
            char hex[3] = {encoded[i+1], encoded[i+2], '\0'};
            decoded += static_cast<char>(strtol(hex, NULL, 16));
            i += 3;
        } else {
            decoded += encoded[i++];
        }
    }
    return decoded;
}

int main() {
    std::string request_line = "GET /%E4%B8%96%E7%95%8C.html HTTP/1.1";
    
    // Extract path (skipping "GET " and " HTTP/1.1")
    size_t start = request_line.find(' ') + 1;
    size_t end = request_line.rfind(' ');
    std::string encoded_uri = request_line.substr(start, end - start);

    std::string decoded_uri = percent_decode(encoded_uri);
    std::cout << "Decoded URI: " << decoded_uri << std::endl;

    return 0;
}
```

### Example Output
```
Decoded URI: /世界.html
```

## 5. Summary
- ✅ **Do not blindly convert Unicode to ASCII**; instead, **decode percent-encoded UTF-8**.
- ✅ **Only methods & HTTP versions must be ASCII**; reject requests if they contain non-ASCII characters.
- ✅ **Normalize Unicode** to NFC form after decoding.
- ✅ **Handle IDNs using Punycode conversion**.

Would you like more details on **Punycode decoding** or **Unicode normalization in C++98**? 🚀



## Percent-Encoding in URIs

Percent-encoding (URL encoding) is used in URIs to encode characters that have special meanings or are not allowed in certain components. A percent-encoded triplet consists of a `%` followed by two hexadecimal digits representing the byte value.

## **Where Percent-Encoding Triplets Are Allowed in a URI**

A URI follows this general structure (as per RFC 3986):

URI         = scheme ":" hier-part [ "?" query ] [ "#" fragment ]

hier-part   = "//" authority path-abempty
            / path-absolute
            / path-rootless
            / path-empty

authority   = [userinfo "@"] host [":" port]
userinfo    = *( unreserved / pct-encoded / sub-delims / ":" )
host        = IP-literal / IPv4address / reg-name
port        = *DIGIT

path        = path-abempty    ; (e.g., `/foo/bar`)
            / path-absolute   ; (e.g., `/foo/bar`)
            / path-rootless   ; (e.g., `foo/bar`)
            / path-empty      ; (e.g., ``)

query       = *( pchar / "/" / "?" )
fragment    = *( pchar / "/" / "?" )

pchar       = unreserved / pct-encoded / sub-delims / ":" / "@"

pct-encoded = "%" HEXDIG HEXDIG
unreserved  = ALPHA / DIGIT / "-" / "." / "_" / "~"
sub-delims  = "!" / "$" / "&" / "'" / "(" / ")"
            / "*" / "+" / "," / ";" / "="

## **Percent-Encoding Rules Per Component**

| **URI Component** | **Allowed Percent-Encoding?** | **Details** |
|-------------------|-------------------------------|-------------|
| **Scheme** (`http`, `https`, `ftp`, etc.) | ❌ **Not allowed** | Only letters, digits, `+`, `-`, `.` are allowed. |
| **Userinfo** (`user:pass@`) | ✅ **Allowed** | Allowed for encoding special characters like `@`, `:` but not strictly necessary. |
| **Host** | ⚠️ **Limited** | Percent-encoding is **not used** for ASCII hostnames. For **Internationalized Domain Names (IDN)**, it is used before Punycode conversion. |
| **Port** (`:8080`) | ❌ **Not allowed** | Must be a numeric value, cannot contain percent-encoded characters. |
| **Path** (`/path/to/resource`) | ✅ **Allowed** | Used for encoding reserved characters (`/`, `?`, `#`, etc.), spaces, and non-ASCII characters. |
| **Query** (`?key=value&other=val`) | ✅ **Allowed** | Used for encoding `&`, `=`, spaces, and other special characters. |
| **Fragment** (`#section1`) | ✅ **Allowed** | Used to encode reserved characters that might affect parsing. |

---

## **Breakdown of Allowed Characters & When They Must Be Percent-Encoded**

| **Character Type** | **Allowed in Path?** | **Allowed in Query?** | **Allowed in Fragment?** |
|-------------------|---------------------|----------------------|----------------------|
| **Unreserved Characters** (`A-Z`, `a-z`, `0-9`, `-`, `.`, `_`, `~`) | ✅ No encoding needed | ✅ No encoding needed | ✅ No encoding needed |
| **Reserved Characters** (`:`, `/`, `?`, `#`, `[`, `]`, `@`) | ⚠️ Must be encoded unless used for structure | ⚠️ Must be encoded unless used for structure | ⚠️ Must be encoded unless used for structure |
| **Sub-delimiters** (`!`, `$`, `&`, `'`, `(`, `)`, `*`, `+`, `,`, `;`, `=`) | ✅ Allowed | ✅ Allowed | ✅ Allowed |
| **Space (` `)** | ❌ Must be encoded (`%20`) | ❌ Must be encoded (`%20` or `+`) | ❌ Must be encoded (`%20`) |
| **Non-ASCII Characters** (e.g., `é`, `ü`, `ç`) | ❌ Must be percent-encoded | ❌ Must be percent-encoded | ❌ Must be percent-encoded |

---

## **Special Considerations**

### **1. Path Segments**
- Each segment (`/segment/`) can have percent-encoded characters.
- The slash `/` is **not encoded** when used as a path separator.
- Example:
  ```
  /some%20folder/file.txt  (Encoding space in "some folder")
  ```

### **2. Query String**
- Query parameters often use `&` and `=`, which should **not** be percent-encoded when separating values.
- Spaces can be encoded as `%20` or `+` in application/x-www-form-urlencoded data.
- Example:
  ```
  ?name=John%20Doe&age=25
  ```

### **3. Fragment**
- Fragments often contain user-defined identifiers like `#section-1`.
- Example:
  ```
  #Introduction%20to%20Web
  ```

---

## **Conclusion**
- Percent-encoding triplets (`%XX`) are **allowed in most URI components** but must follow specific rules.
- The **scheme and port** do **not** allow percent-encoding.
- The **host** does not use percent-encoding except in IDN cases.
- The **path, query, and fragment** allow percent-encoding for reserved and non-ASCII characters.

# How to Determine If a URI is Unicode or UTF-8

## 1. Understanding URI Encoding
A URI (Uniform Resource Identifier) can be encoded in different ways, typically using:

- **Percent-Encoding (URL encoding)**: Used to encode special characters (e.g., `%20` for space).
- **UTF-8 Encoding**: A common encoding standard for handling Unicode characters in URIs.
- **Raw Unicode**: Some systems may improperly store Unicode characters without encoding them.

## 2. Steps to Identify URI Encoding

### **Step 1: Check for Percent-Encoding**
If the URI contains percent-encoded characters (e.g., `%E2%9C%93`), decode them to determine if they represent UTF-8 sequences.

- Example:
  - `%E2%9C%93` → Decodes to `✓` (UTF-8 checkmark character)
  - `%C3%A9` → Decodes to `é`

### **Step 2: Attempt UTF-8 Decoding**
If the URI contains raw bytes, check if they form a valid UTF-8 sequence.

- **Valid UTF-8 Sequence:** If decoding succeeds without errors, it is UTF-8.
- **Invalid UTF-8 Sequence:** If decoding results in errors, the URI may be using another encoding (e.g., ISO-8859-1).

### **Step 3: Detect Raw Unicode**
If the URI contains unencoded Unicode characters (e.g., `https://example.com/你好`), it may not be properly encoded for transport.

## 3. Example Scenarios

### **UTF-8 Encoded URI**

```
https://example.com/%E4%BD%A0%E5%A5%BD
```
- Percent-decoded: `你好`
- Successfully interprets as UTF-8 → **UTF-8 encoded**

### **Raw Unicode URI (Not Properly Encoded)**
```
https://example.com/你好
```
- Appears as Unicode directly → **May cause issues in transport**

### **Non-UTF-8 Encoded URI (ISO-8859-1 Example)**
```
https://example.com/%E9
```
- If `%E9` does not form a valid UTF-8 character → **Not UTF-8**

## 4. Programmatic Detection
If working in C++, you can analyze the URI:

- **Percent Decode** the URI.
- **Verify UTF-8 Validity** using a UTF-8 validation function.

Example (conceptual approach):
```cpp
bool isValidUTF8(const std::string &str);
std::string decodePercentEncoding(const std::string &uri);

std::string decoded = decodePercentEncoding(uri);
if (isValidUTF8(decoded)) {
    std::cout << "URI is UTF-8 encoded." << std::endl;
} else {
    std::cout << "URI may be in a different encoding." << std::endl;
}
```

## 5. Conclusion
- **If a URI is percent-encoded**, decode and check if it forms valid UTF-8.
- **If a URI contains raw Unicode characters**, it might not be properly encoded.
- **Use programmatic checks** to verify encoding validity.

By following these steps, you can determine if a URI is using Unicode or UTF-8 encoding effectively.
