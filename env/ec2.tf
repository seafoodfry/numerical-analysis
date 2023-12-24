data "aws_ami" "ubuntu" {
  most_recent = true
  filter {
    name   = "name"
    values = ["ubuntu/images/hvm-ssd/ubuntu-jammy-22.04-amd64-server-*"]
  }
  filter {
    name   = "virtualization-type"
    values = ["hvm"]
  }
  owners = ["099720109477"] # Canonical
}

locals {
  vars = {
    #some_address = aws_instance.some.private_ip
  }
}

resource "aws_spot_instance_request" "ubuntu" {
  spot_price           = "0.05" # https://aws.amazon.com/ec2/spot/pricing/
  spot_type            = "one-time"
  wait_for_fulfillment = true

  ami           = data.aws_ami.ubuntu.id
  instance_type = "t3.xlarge"

  vpc_security_group_ids = [aws_security_group.ssh.id]
  availability_zone      = "us-east-2c"
  key_name               = var.ec2_key_name
  root_block_device {
    delete_on_termination = true
    encrypted             = true
    volume_type           = "gp3"
    volume_size           = 8
  }
  metadata_options {
    http_tokens = "required"
  }

  user_data = base64encode(templatefile("${path.module}/setup.sh.tpl", local.vars))

  tags = {
    Name = "ubuntu"
  }
}

output "ubuntu" {
  value       = aws_spot_instance_request.ubuntu.public_dns
  description = "Public DNS"
}
