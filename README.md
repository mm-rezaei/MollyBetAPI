
# MollyBet API (Client Side)

## MollyBet Overview

**MollyBet** is a highly advanced liquidity pooling execution platform tailored specifically for professional traders who engage in high-stakes betting. The platform is designed to give these traders a competitive advantage in a rapidly evolving and sophisticated market environment. By aggregating liquidity from various sources, MollyBet enables traders to execute large-volume bets efficiently and effectively.

## MollyBet API

The **MollyBet API** extends the capabilities of the MollyBet platform by providing programmatic access to its features. This API offers a more extensive set of functionalities than those available through the MollyBet website, allowing developers and traders to integrate and automate their trading strategies with greater precision and flexibility. By leveraging the API, users can interact with the platform at a deeper level, enhancing their trading operations in the high-stakes market.

## MollyBet API Challenge

This project was developed as part of a technical challenge for a position at MollyBet. The task involved using modern C++ to interact with the MollyBet API by making an HTTP request to log in, connecting to a WebSocket stream, processing incoming messages, and extracting specific data. The project includes a self-contained Dockerfile for easy setup and testing, along with a detailed README to explain the code structure and design choices. This challenge demonstrates proficiency in API integration, WebSocket communication, and clean, modern C++ coding practices. This code was developed in less than three days of part-time work, demonstrating my ability to quickly deliver high-quality, efficient solutions using industry-standard libraries, tools, and techniques.

## Libraries and Tools Used 

For this challenge, I utilized several key libraries and tools to address the requirements effectively:

- **Boost.Asio:** Chosen for its robust and efficient handling of asynchronous networking operations, essential for managing HTTP requests and WebSocket connections.
- **Boost.Beast:** Used for its seamless integration with Boost.Asio, providing a powerful and flexible way to handle both HTTP and WebSocket protocols.
- **Boost.JSON:** Leveraged to efficiently parse and manage JSON data, ensuring smooth processing of the API responses.
- **OpenSSL:** Incorporated to secure the communication channels, ensuring all data exchanges were encrypted and protected.
- **Modern C++ (C++17/20):** Advanced features of modern C++ were used to enhance code readability, maintainability, and performance, showcasing best practices in contemporary software development.
- **Docker:** Employed to create a self-contained environment, ensuring the code could be easily tested and deployed across different systems with consistent behavior.

## Class Descriptions and Responsibilities

### 1. **ConnectionInfo.h**
   - **Description**: Encapsulates the connection information required to establish a connection to the MollyBet API, such as the host and port. This class provides a straightforward way to manage and pass around connection details within the application.
   
### 2. **SessionInfo.h**
   - **Description**: Stores session information, specifically the session code, which is essential for maintaining authenticated communication with the MollyBet API. This class ensures that the session context is easily accessible and manageable across different components of the application.
   
### 3. **ResponseStatus.h**
   - **Description**: A simple enumeration defining possible response statuses (e.g., `Ok` or `Error`). This enumeration standardizes the handling of API responses throughout the application, making the code more readable and easier to manage.
   
### 4. **StreamAuxiliary.h**
    - **Description**: A template class that manages the initialization and operation of a network stream, including handling SSL contexts and resolving endpoints. It serves as a utility for managing low-level stream operations needed for both HTTP and WebSocket connections, ensuring that the network layers are correctly set up and maintained.
	
### 5. **ConnectionServiceBase.h**
   - **Description**: An abstract base class providing an interface for connection services. It defines essential methods for opening, closing, and checking the status of a connection, ensuring a consistent interface for different types of connection services.

### 6. **SynchronousConnectionService.h**
    - **Description**: Manages synchronous HTTP connections using Boost.Asio and Boost.Beast. It handles the opening and closing of connections, as well as executing HTTP requests and processing the corresponding responses in a synchronous manner. This class is central to handling one-off HTTP requests that require immediate results.
	
### 7. **AsynchronousConnectionService.h**
   - **Description**: Manages asynchronous WebSocket connections using Boost.Asio and Boost.Beast. It handles the opening, closing, and status checking of WebSocket connections and processes asynchronous notifications received from the server. The class includes a method, ListenToAsynchronousNotification, which processes incoming notifications in the sequence they are received. This design assumes that maintaining the order of processing is critical. If out-of-order processing were acceptable, the implementation could be optimized further by removing the locking mechanism, potentially improving performance.
   
### 8. **SynchronousRequestBase.h**
    - **Description**: An abstract base class for creating and processing synchronous HTTP requests. It defines the structure for generating requests, including pre-processing and post-processing steps, and enforces the implementation of request creation and processing logic in derived classes. This provides a standardized approach to handling different types of HTTP requests.
	
### 9. **LoginSynchronousRequest.h**
   - **Description**: Manages the creation and processing of a synchronous HTTP login request to the MollyBet API. It constructs the request using the provided username and password, ensuring that the necessary authentication data is sent correctly to obtain a session token.
   
### 10. **SynchronousResponseBase.h**
    - **Description**: Serves as a base for handling synchronous HTTP responses. It parses the HTTP response, extracting the status code and other relevant data, and provides utility methods to retrieve the response's HTTP status and custom response status. This class ensures that responses are consistently processed and interpreted across the application.
	
### 11. **LoginSynchronousResponse.h**
   - **Description**: Handles the response from a synchronous login request. This class extracts and stores the session code returned by the MollyBet API, which is crucial for establishing authenticated WebSocket connections in subsequent operations.
   
### 12. **AsynchronousNotificationAuxiliaryBase.h**
   - **Description**: An abstract base class that defines the interface for processing asynchronous notifications. Derived classes must implement the `ProcessNotification` method, which is used to handle messages received via WebSocket. This provides a flexible framework for dealing with different types of asynchronous notifications.
   
### 13. **SyncNotificationAuxiliary.h**
    - **Description**: Extends `AsynchronousNotificationAuxiliaryBase` to process notifications received via WebSocket asynchronously. It specifically handles event messages to extract and store distinct "competition_name" values, terminating when a "sync" message is encountered. This class is essential for managing and interpreting real-time data streams from the API.
	
### 14. **MollybetTechChallenge.cpp**
   - **Description**: The main application file that integrates all components to execute the required operations. It handles the login process, establishes WebSocket connections, and processes received notifications to extract and display distinct competition names. This file serves as the entry point and orchestrates the flow of the program.

## How to Execute the Code

### 1. Running the Code with Visual Studio

If you prefer to run the code in a Windows environment, I have provided a Visual Studio solution with the project. To execute the source code using Visual Studio, follow these steps:

1. **Download the Required Libraries**:
   - **Boost**: Download the Boost library from the [official Boost website](https://www.boost.org/). Ensure you download the version compatible with your system.
   - **OpenSSL**: Download and install OpenSSL from the [official OpenSSL website](https://www.openssl.org/).

2. **Set Up the Visual Studio Solution**:
   - After downloading the Boost and OpenSSL libraries, you need to configure the Visual Studio project to reference these libraries and headers.
   - Open the solution in Visual Studio.

3. **Build and Run**:
   - After setting the correct paths, you can build the project by selecting **Build Solution** from the Build menu.
   - Once the build completes successfully, run the application from Visual Studio.

### 2. Running the Code with Docker

Alternatively, you can use Docker to build and execute the code in a Linux environment without worrying about setting up dependencies on your local machine. The Docker setup ensures that all required libraries are automatically downloaded and configured.

**Dockerfile Content**:
```dockerfile
FROM ubuntu:22.04

# Install essential build tools and dependencies
RUN apt-get update && apt-get install -y \ build-essential \ wget \ git \ libssl-dev \ && apt-get clean

# Download and install Boost libraries
RUN wget https://boostorg.jfrog.io/artifactory/main/release/1.82.0/source/boost_1_82_0.tar.gz \
    && tar xzf boost_1_82_0.tar.gz \
    && cd boost_1_82_0 \
    && ./bootstrap.sh --with-libraries=system,thread,json \
    && ./b2 install \
    && cd .. \
    && rm -rf boost_1_82_0*

# Copy application source code
COPY . /app

# Set working directory
WORKDIR /app

# Set LD_LIBRARY_PATH to include /usr/local/lib
ENV LD_LIBRARY_PATH="/usr/local/lib:$LD_LIBRARY_PATH"

# Compile the application
RUN g++ -o mollybettechchallenge MollybetTechChallenge.cpp -lboost_system -lboost_thread -lboost_json -lpthread -lssl -lcrypto -std=c++17 -O2

# Set default command
CMD ["./mollybettechchallenge"]
```

**Steps to Execute with Docker**:

1. **Build the Docker Image**:
   - Navigate to the directory containing the Dockerfile.
   - Run the following command to build the Docker image:
     ```sh
     docker build -t mollybet-api-client .
     ```

2. **Run the Docker Container**:
   - After the image is built, you can run the application inside a Docker container using the following command:
     ```sh
     docker run -it --rm mollybet-api-client
     ```

This method ensures that you have a consistent environment across different systems and simplifies dependency management.

## Limitations and Future Improvements

Given the tight timeframe for this challenge, there may be some misunderstandings or misinterpretations of the MollyBet API based on the documentation available on the MollyBet website. While every effort was made to ensure accuracy and compliance with the API's intended use, certain aspects may need refinement or adjustment with further clarification and testing.
With additional time, this project could be further enhanced to make it more professional and efficient. Potential improvements include optimizing performance, refining the handling of asynchronous notifications, and enhancing the overall code structure to better align with best practices in high-performance C++ development. These enhancements would allow for a more polished and robust solution that fully leverages the capabilities of the MollyBet API.

## License

This project is licensed under the MIT License - see the [LICENSE] file for details.

## Libraries Used

This project uses the following libraries:

- [Boost](https://www.boost.org/users/license.html) - Boost Software License 1.0
- [OpenSSL](https://www.openssl.org/source/license.html) - OpenSSL License and SSLeay License

## Conclusion

This project showcases the integration of modern C++ techniques with the MollyBet API to create a robust and efficient client-side application. By leveraging advanced libraries such as Boost.Asio, Boost.Beast, and OpenSSL, the project efficiently handles both synchronous HTTP requests and asynchronous WebSocket communications. The design choices, such as maintaining processing order in asynchronous notifications, highlight a thoughtful approach to ensuring data integrity and reliability in real-time operations. This project, developed in under three days of part-time work, stands as a testament to the ability to deliver high-quality, maintainable, and scalable solutions within tight deadlines.