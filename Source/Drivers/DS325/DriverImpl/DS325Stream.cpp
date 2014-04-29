/*****************************************************************************
*                                                                            *
*  OpenNI 2.x                                                                *
*  Copyright (C) 2012 PrimeSense Ltd.                                        *
*  Copyright (C) 2014 SIProp.org http://www.siprop.org/                      *
*                                                                            *
*  This file is part of OpenNI.                                              *
*                                                                            *
*  Licensed under the Apache License, Version 2.0 (the "License");           *
*  you may not use this file except in compliance with the License.          *
*  You may obtain a copy of the License at                                   *
*                                                                            *
*      http://www.apache.org/licenses/LICENSE-2.0                            *
*                                                                            *
*  Unless required by applicable law or agreed to in writing, software       *
*  distributed under the License is distributed on an "AS IS" BASIS,         *
*  WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.  *
*  See the License for the specific language governing permissions and       *
*  limitations under the License.                                            *
*                                                                            *
*****************************************************************************/
#include "DS325Stream.h"
#include "DS325Config.h"


OniStatus DS325Stream::getProperty(int propertyId, void* data, int* pDataSize) {
	if(propertyId == ONI_STREAM_PROPERTY_VIDEO_MODE) {
		if(*pDataSize != sizeof(OniVideoMode)) {
			printf("Unexpected size: %d != %d\n", *pDataSize, (int)sizeof(OniVideoMode));
			return ONI_STATUS_ERROR;
		}
		return GetVideoMode((OniVideoMode*)data);
	}

	return ONI_STATUS_NOT_IMPLEMENTED;
}

OniStatus DS325Stream::setProperty(int propertyId, const void* data, int dataSize) {
	if(propertyId == ONI_STREAM_PROPERTY_VIDEO_MODE) {
		if(dataSize != sizeof(OniVideoMode)) {
			printf("Unexpected size: %d != %d\n", dataSize, (int)sizeof(OniVideoMode));
			return ONI_STATUS_ERROR;
		}
		return SetVideoMode((OniVideoMode*)data);
	}

	return ONI_STATUS_NOT_IMPLEMENTED;
}

XN_THREAD_PROC DS325Stream::threadFunc(XN_THREAD_PARAM pThreadParam) {
	DS325Stream* pStream = (DS325Stream*)pThreadParam;
	pStream->m_running = true;
	pStream->Mainloop();

	XN_THREAD_PROC_RETURN(XN_STATUS_OK);
}


int DS325Stream::errno_exit(const char *s) {
	printf("%s error %d, %s\n", s, errno, strerror (errno));
	return ERROR_LOCAL;
}

int DS325Stream::xioctl(int fd, int request, void *arg) {
	int r;

	do r = ioctl (fd, request, arg);
	while (-1 == r && EINTR == errno);

	return r;
}


int DS325Stream::send_xu(int query, unsigned char* data) {

    struct uvc_xu_control_query ctrl;

    ctrl.unit = UVC_PU_HUE_CONTROL;
    ctrl.selector = 2;
    ctrl.query = query;
    ctrl.size = 7;
    ctrl.data = data;

    if (ioctl(fd,  UVCIOC_CTRL_QUERY, &ctrl) < 0) errno_exit("ctrl error\n");

    return 0;
}


OniStatus DS325Stream::start_stream(int device_id, int width, int height) {
	int ret;

	fd = -1;
	n_buffers = 0;

	ret = open_device(device_id);

	if(ret != ERROR_LOCAL) {
		ret = init_device(width, height);
	} else {
		return ONI_STATUS_ERROR;
	}
	if(ret != ERROR_LOCAL) {
		ret = start_capturing();
		if(ret != SUCCESS_LOCAL) {
			stop_capturing();
			uninit_device ();
			close_device ();
			return ONI_STATUS_ERROR;
		}
	} else {
		return ONI_STATUS_ERROR;
	}
	xnOSCreateThread(threadFunc, this, &m_threadHandle);
	return ONI_STATUS_OK;
}

void DS325Stream::stop_stream() {
	m_running = false;
	xnOSSleep(300);
	if(fd == -1) return;

	stop_capturing();
	uninit_device();
	close_device();
	fd = -1;
}


int DS325Stream::open_device(int device_id) {
	struct stat st;

	sprintf(dev_name, "/dev/video%d", device_id);

	if (-1 == stat (dev_name, &st)) {
		errno_exit("Cannot identify");
		return ERROR_LOCAL;
	}

	if (!S_ISCHR (st.st_mode)) {
		return ERROR_LOCAL;
	}

	fd = open(dev_name, O_RDWR | O_NONBLOCK, 0);

	if (-1 == fd) {
		errno_exit("Cannot open");
		return ERROR_LOCAL;
	}
	return SUCCESS_LOCAL;
}


int DS325Stream::init_device(int width, int height) {
	struct v4l2_capability cap;
	struct v4l2_cropcap cropcap;
	struct v4l2_crop crop;
	struct v4l2_format fmt;
	unsigned int min;

	if(-1 == xioctl(fd, VIDIOC_QUERYCAP, &cap)) {
		if (EINVAL == errno) {
			printf("%s is no V4L2 device", dev_name);
			return ERROR_LOCAL;
		} else {
			return errno_exit("VIDIOC_QUERYCAP");
		}
	}

	if(!(cap.capabilities & V4L2_CAP_VIDEO_CAPTURE)) {
		printf("%s is no video capture device", dev_name);
		return ERROR_LOCAL;
	}

	if(!(cap.capabilities & V4L2_CAP_STREAMING)) {
		printf("%s does not support streaming i/o", dev_name);
		return ERROR_LOCAL;
	}

	CLEAR(cropcap);

	cropcap.type = V4L2_BUF_TYPE_VIDEO_CAPTURE;

	if(0 == xioctl(fd, VIDIOC_CROPCAP, &cropcap)) {
		crop.type = V4L2_BUF_TYPE_VIDEO_CAPTURE;
		crop.c = cropcap.defrect;
		if(-1 == xioctl(fd, VIDIOC_S_CROP, &crop)) {
			switch (errno) {
				case EINVAL:
					break;
				default:
					break;
			}
		}
	}

	CLEAR(fmt);

	fmt.type                = V4L2_BUF_TYPE_VIDEO_CAPTURE;

	fmt.fmt.pix.width       = width;
	fmt.fmt.pix.height      = height;

	fmt.fmt.pix.pixelformat = V4L2_PIX_FMT_YUYV;
	fmt.fmt.pix.field       = V4L2_FIELD_INTERLACED;

	if(-1 == xioctl(fd, VIDIOC_S_FMT, &fmt))
		return errno_exit("VIDIOC_S_FMT");

	min = fmt.fmt.pix.width * 2;
	if(fmt.fmt.pix.bytesperline < min)
		fmt.fmt.pix.bytesperline = min;
	min = fmt.fmt.pix.bytesperline * fmt.fmt.pix.height;
	if(fmt.fmt.pix.sizeimage < min)
		fmt.fmt.pix.sizeimage = min;

	return init_mmap();

}

int DS325Stream::init_mmap(void) {
	struct v4l2_requestbuffers req;

	CLEAR(req);

	req.count               = 4;
	req.type                = V4L2_BUF_TYPE_VIDEO_CAPTURE;
	req.memory              = V4L2_MEMORY_MMAP;

	if(-1 == xioctl(fd, VIDIOC_REQBUFS, &req)) {
		if(EINVAL == errno) {
			printf("%s does not support memory mapping", dev_name);
			return ERROR_LOCAL;
		} else {
			return errno_exit("VIDIOC_REQBUFS");
		}
	}

	if(req.count < 2) {
		printf("Insufficient buffer memory on %s", dev_name);
		return ERROR_LOCAL;
 	}

	buffers = calloc(req.count, sizeof (*buffers));

	if(!buffers) {
		printf("Out of memory");
		return ERROR_LOCAL;
	}

	for(n_buffers = 0; n_buffers < req.count; ++n_buffers) {
		struct v4l2_buffer buf;

		CLEAR (buf);

		buf.type        = V4L2_BUF_TYPE_VIDEO_CAPTURE;
		buf.memory      = V4L2_MEMORY_MMAP;
		buf.index       = n_buffers;

		if(-1 == xioctl(fd, VIDIOC_QUERYBUF, &buf))
			return errno_exit("VIDIOC_QUERYBUF");

		buffers[n_buffers].length = buf.length;
		buffers[n_buffers].start =
		mmap(NULL ,
			buf.length,
			PROT_READ | PROT_WRITE,
			MAP_SHARED,
			fd, buf.m.offset);

		if(MAP_FAILED == buffers[n_buffers].start)
			return errno_exit("mmap");
	}

	return SUCCESS_LOCAL;
}


int DS325Stream::start_capturing(void) {
	unsigned int i;
	enum v4l2_buf_type type;

	for(i = 0; i < n_buffers; ++i) {
		struct v4l2_buffer buf;

		CLEAR (buf);

		buf.type        = V4L2_BUF_TYPE_VIDEO_CAPTURE;
		buf.memory      = V4L2_MEMORY_MMAP;
		buf.index       = i;

		if(-1 == xioctl(fd, VIDIOC_QBUF, &buf))
			return errno_exit("VIDIOC_QBUF");
	}

	type = V4L2_BUF_TYPE_VIDEO_CAPTURE;

	if(-1 == xioctl(fd, VIDIOC_STREAMON, &type))
		return errno_exit("VIDIOC_STREAMON");

	return SUCCESS_LOCAL;
}


int DS325Stream::read_frame_once() {
	for(;;) {
		fd_set fds;
		struct timeval tv;
		int r;

		FD_ZERO (&fds);
		FD_SET (fd, &fds);

		tv.tv_sec = 2;
		tv.tv_usec = 0;

		r = select(fd + 1, &fds, NULL, NULL, &tv);

		if(-1 == r) {
			if(EINTR == errno)
				continue;

			return errno_exit("select");
		}

		if(0 == r) {
			printf("select timeout");
			return ERROR_LOCAL;

		}

		if(read_frame() == 1)
			break;

	}

	return SUCCESS_LOCAL;

}

int DS325Stream::read_frame(void) {

	struct v4l2_buffer buf;
	unsigned int i;

	CLEAR (buf);

	buf.type = V4L2_BUF_TYPE_VIDEO_CAPTURE;
	buf.memory = V4L2_MEMORY_MMAP;

	if(-1 == xioctl(fd, VIDIOC_DQBUF, &buf)) {
		switch(errno) {
			case EAGAIN:
				return 0;
			case EIO:
			default:
				return errno_exit("VIDIOC_DQBUF");
		}
	}

	assert(buf.index < n_buffers);

	set_image((unsigned char*)(buffers[buf.index].start), buf.length);

	if(-1 == xioctl (fd, VIDIOC_QBUF, &buf))
		return errno_exit("VIDIOC_QBUF");

	return 1;
}

int DS325Stream::stop_capturing(void) {
	enum v4l2_buf_type type;

	type = V4L2_BUF_TYPE_VIDEO_CAPTURE;

	if(-1 == xioctl(fd, VIDIOC_STREAMOFF, &type))
		return errno_exit("VIDIOC_STREAMOFF");

	return SUCCESS_LOCAL;

}

int DS325Stream::uninit_device(void) {
	unsigned int i;

	for(i = 0; i < n_buffers; ++i)
		if(-1 == munmap(buffers[i].start, buffers[i].length))
			return errno_exit("munmap");

	free(buffers);

	return SUCCESS_LOCAL;
}

int DS325Stream::close_device(void) {
	if(-1 == close(fd)) {
		fd = -1;
		return errno_exit("close");
	}

	fd = -1;
	return SUCCESS_LOCAL;
}

