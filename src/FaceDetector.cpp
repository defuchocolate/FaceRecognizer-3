#include <FaceDetector.hpp>

#include <iostream>

FaceDetector::FaceDetector(const std::string& aPathToHaarCascade, int aImageWidth, int aImageHeight) :
	mIsValid(true),
	mPathToHaarCascade(aPathToHaarCascade),
	mHaarCascade(),
	mImageWidth(aImageWidth),
	mImageHeight(aImageHeight)
{
	try
	{
		mHaarCascade.load(aPathToHaarCascade);
	}
	catch(cv::Exception& e)
	{
		std::cerr << "Instantiation of FaceDetector failed!" << std::endl;
		mIsValid = false;
	}
}

FaceDetector::~FaceDetector()
{

}

FaceDetector::operator bool() const
{
	return mIsValid;
}

bool FaceDetector::DetectFace(const cv::Mat& aVideoFrame, std::vector<cv::Mat>& aFoundFaces)
{
	cv::Mat original = aVideoFrame.clone();

	// Convert the current frame to grayscale:
	cv::Mat gray;
	cvtColor(original, gray, CV_BGR2GRAY);

	// Find the faces in the frame:
	std::vector<cv::Rect_<int>> faces;
	mHaarCascade.detectMultiScale(gray, faces);

	for (auto currFace : faces)
	{
		// Crop the face from the image. So simple with OpenCV C++:
		cv::Mat face = gray(currFace);
		// Resizing the face is necessary for Eigenfaces and Fisherfaces. You can easily
		// verify this, by reading through the face recognition tutorial coming with OpenCV.
		// Resizing IS NOT NEEDED for Local Binary Patterns Histograms, so preparing the
		// input data really depends on the algorithm used.
		//
		// I strongly encourage you to play around with the algorithms. See which work best
		// in your scenario, LBPH should always be a contender for robust face recognition.
		//
		// Since I am showing the Fisherfaces algorithm here, I also show how to resize the
		// face you have just found:
		cv::Mat face_resized;
		cv::resize(face, face_resized, cv::Size(mImageWidth, mImageHeight), 1.0, 1.0, cv::INTER_CUBIC);

		aFoundFaces.push_back(face_resized);
		// Now perform the prediction, see how easy that is:
		/*int prediction = model->predict(face_resized);
		// And finally write all we've found out to the original image!
		// First of all draw a green rectangle around the detected face:
		rectangle(original, face_i, CV_RGB(0, 255,0), 1);
		// Create the text we will annotate the box with:
		string box_text = format("Prediction = %d", prediction);
		// Calculate the position for annotated text (make sure we don't
		// put illegal values in there):
		int pos_x = std::max(face_i.tl().x - 10, 0);
		int pos_y = std::max(face_i.tl().y - 10, 0);
		// And now put it into the image:
		putText(original, box_text, Point(pos_x, pos_y), FONT_HERSHEY_PLAIN, 1.0, CV_RGB(0,255,0), 2.0);*/
	}

	if (0 < aFoundFaces.size())
	{
		return true;
	}
	return false;
}
